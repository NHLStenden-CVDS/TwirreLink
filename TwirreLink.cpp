/*
 * TwirreLink.cpp
 *
 *  Created on: Apr 9, 2015
 *      Author: root
 */

#include <string>
#include <map>
#include <exception>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "TwirreLink.h"

#include <chrono>

#include "TwirreSerial/TwirreSerial.h"

using namespace std;
using namespace twirre;

#ifdef TWIRRELINK_DEBUG_MAIN
int main()
{
	TwirreSerial ifx("/dev/ttyACM0");	//connect to arduino DUE running TwirreLink firmware
	TwirreLink twirre(ifx);

	bool haveNaza = twirre.haveActuator("naza");

	while (true)
	{
		//get imu values
		auto imuvals = twirre.getSensor("myAHRS+")[{"pitch", "yaw", "roll"}];
		//print the imu values
		for (auto & val : imuvals)
		{
			std::cout << val.first << ":\t" << val.second->as_int16_t() << "\t";
		}
		//print sonar height
		auto& distvals = twirre.getSensor("sonar1")["distanceValues"];
		std::cout << "alt:\t" << distvals.as_int16_t();
		std::cout << std::endl;

		//send control to flight controller based on IMU values
		if (haveNaza)
		{
			auto& naza = twirre.getActuator("naza");

			//set the naza actuator values
			naza["pitch"] = (imuvals["pitch"]->as_float() / 180.0f);
			naza["yaw"] = (imuvals["yaw"]->as_float() / 180.0f);
			naza["roll"] = (imuvals["roll"]->as_float() / 180.0f);
			naza["gaz"] = (distvals.as_float(0) / 100.0f) - 1.0f;
			naza["timeout"] = 10000;	//timeout in ms, arduino PWM output will reset to [0,0,0,0] if no new actuation is performed within this period

			naza.Actuate();//send updated values to the naza actuator
		}
	}
}
#endif

namespace twirre
{

	TwirreLink::TwirreLink()
	{

	}

	TwirreLink::~TwirreLink()
	{
		stopLogging();

		//remove all links
		for (auto prov : _providers)
		{
			prov->removeLink(this);
		}
	}

	TwirreLink::TwirreLink(DeviceProvider& prov)
	{
		addProvider(prov);
	}

	TwirreLink::TwirreLink(const std::vector<DeviceProvider*>& provs)
	{
		for (const auto prov : provs)
		{
			if (prov) addProvider(*prov, false);
		}
		notifyChange();
	}

	TwirreLink::TwirreLink(const TwirreLink & other)
	{
		for (const auto prov : other._providers)
		{
			addProvider(*prov, false);
		}
		notifyChange();
	}

	TwirreLink::TwirreLink(TwirreLink && other)
	{
		for (const auto prov : other._providers)
		{
			addProvider(*prov, false);
		}
		notifyChange();
	}

	TwirreLink & TwirreLink::operator =(const TwirreLink & other)
	{
		//remove all links
		for (auto prov : _providers)
		{
			prov->removeLink(this);
		}

		_providers = other._providers;

		//build new links
		for (auto prov : _providers)
		{
			prov->addLink(this);
		}
		notifyChange();

		return *this;
	}

	TwirreLink & TwirreLink::operator =(TwirreLink && other)
	{
		//remove all links
		for (auto prov : _providers)
		{
			prov->removeLink(this);
		}

		_providers = other._providers;

		//build new links
		for (auto prov : _providers)
		{
			prov->addLink(this);
		}
		notifyChange();

		return *this;
	}

	const std::map<string, Actuator*> & TwirreLink::getActuators()
	{
		return _actuatorList;
	}

	const std::map<string, Sensor*> & TwirreLink::getSensors()
	{
		return _sensorList;
	}

	void TwirreLink::addProvider(DeviceProvider& prov, bool update)
	{
		//prevent duplicate insertion
		if(_providers.find(&prov) != _providers.end()) return;

		//create bidirectional link
		prov.addLink(this);
		_providers.insert(&prov);

		if (update) notifyChange();
	}

	void TwirreLink::removeProvider(DeviceProvider& prov)
	{
		//check if provider was actually added
		if(_providers.find(&prov) == _providers.end()) return;



		//break bidirectional link
		prov.removeLink(this);
		removeLink(&prov);

		notifyChange();
	}

	bool TwirreLink::haveSensor(const string & sensorName) const
	{
		return (_sensorList.find(sensorName) != _sensorList.end());
	}

	bool TwirreLink::haveActuator(const string & actuatorName) const
	{
		return (_actuatorList.find(actuatorName) != _actuatorList.end());
	}

	Actuator& TwirreLink::getActuator(const string & actuatorName)
	{
		if (_actuatorList.find(actuatorName) == _actuatorList.end())
		{
			throw runtime_error("GetActuator: no actuator with that name");
		}
		return *_actuatorList.at(actuatorName);
	}

	Sensor& TwirreLink::getSensor(const string & sensorName)
	{
		if (_sensorList.find(sensorName) == _sensorList.end())
		{
			throw runtime_error("GetSensor: no sensor with name: " + sensorName);
		}
		return *_sensorList.at(sensorName);
	}

	void TwirreLink::notifyChange()
	{
		_actuatorList.clear();
		_sensorList.clear();
		for (const auto prov : _providers)
		{
			auto& actuators = prov->getActuators();
			_actuatorList.insert(actuators.begin(), actuators.end());
			auto& sensors = prov->getSensors();
			_sensorList.insert(sensors.begin(), sensors.end());
		}

		logDevices();
	}

	void TwirreLink::removeLink(DeviceProvider * which)
	{
		_providers.erase(which);
	}

	void TwirreLink::startLogging(const string & loggingPath)
	{
		if(_logger != nullptr) return;

		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "twirrelog_%Y-%m-%d_%H-%M-%S");
		std::string datetime = ss.str();

		string outpath = loggingPath + datetime + ".tlog";
		string binpath = loggingPath + datetime + ".tbin";
		std::cout << "creating logger at " << outpath << std::endl;
		_logger = new TwirreLogger(outpath, binpath);

		logDevices();

		for(auto & provider : _providers)
		{
			provider->addLogger(_logger);
		}
	}

	bool TwirreLink::stopLogging(void)
	{
		if(_logger == nullptr) return false;

		for(auto & provider : _providers)
		{
			provider->removeLogger(_logger);
		}

		delete _logger;
		_logger = nullptr;
		return true;
	}

	void TwirreLink::logDevices(void)
	{
		if(_logger == nullptr) return;

		//log actuators
		_logger->logActuators(_actuatorList);
		//log sensors
		_logger->logSensors(_sensorList);
	}



} /* namespace twirre */

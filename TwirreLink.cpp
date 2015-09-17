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

#include "TwirreLink.h"
#include "TwirreSerial/TwirreSerial.h"

using namespace std;
using namespace twirre;

#ifdef TWIRRELINK_DEBUG_MAIN
int main()
{
	TwirreSerial ifx("/dev/ttyACM0");
	TwirreLink twirre(ifx);

	bool haveNaza = twirre.haveActuator("naza");

	while (true)
	{
		auto imuvals = twirre.getSensor("myAHRS+")[
		{	"pitch", "yaw", "roll"}];
		//print the imu values
		for (auto & val : imuvals)
		{
			std::cout << val.first << ":\t" << val.second->as_int16_t() << "\t";
		}
		auto& distvals = twirre.getSensor("sonar1")["distanceValues"];
		std::cout << "alt:\t" << distvals.as_int16_t();

		std::cout << std::endl;

		if (haveNaza)
		{

			auto& naza = twirre.getActuator("naza");

			//set the naza actuator values
			naza["pitch"] = (imuvals["pitch"]->as_float() / 180.0f);
			naza["yaw"] = (imuvals["yaw"]->as_float() / 180.0f);
			naza["roll"] = (imuvals["roll"]->as_float() / 180.0f);
			naza["gaz"] = (distvals.as_float(0) / 100.0f) - 1.0f;
			naza["timeout"] = 10000;

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
			throw runtime_error("GetSensor: no sensor with that name");
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
	}

	void TwirreLink::removeLink(DeviceProvider * which)
	{
		_providers.erase(which);
	}

} /* namespace twirre */
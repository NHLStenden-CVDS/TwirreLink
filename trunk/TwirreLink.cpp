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

int main()
{
	TwirreLink twirre;
	TwirreSerial ifx("/dev/ttyACM0");
	twirre.addProvider(ifx);

	bool haveNaza = twirre.haveActuator("naza");

	while (true)
	{
		auto imuvals = twirre.getSensor("myAHRS+")[{ "pitch", "yaw", "roll" }];
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

			naza.Actuate(); //send updated values to the naza actuator
		}
	}
}

namespace twirre
{

	TwirreLink::TwirreLink()
	{

	}

	const std::map<string, Actuator*> & TwirreLink::getActuators()
	{
		return _actuatorList;
	}


	const std::map<string, Sensor*> & TwirreLink::getSensors()
	{
		return _sensorList;
	}

	void TwirreLink::addProvider(DeviceProvider& prov)
	{
		auto& sensors = prov.getSensors();
		for (const auto& pair : sensors)
		{
			const string basename = pair.first;
			string name = basename;
			int ctr = 1;

			//append a number to the name when a sensor with that name already exists
			while (haveSensor(name))
			{
				string name = basename;

				char buf[64];
				sprintf(buf, "_%d", ctr++);
				name.append(buf);
			}

			_sensorList[name] = pair.second;
		}

		auto& actuators = prov.getActuators();
		for (const auto& pair : actuators)
		{
			const string basename = pair.first;
			string name = basename;
			int ctr = 1;

			//append a number to the name when an actuator with that name already exists
			while (haveActuator(name))
			{
				string name = basename;

				char buf[64];
				sprintf(buf, "_%d", ctr++);
				name.append(buf);
			}

			_actuatorList[name] = pair.second;
		}
	}

	/**
	 *
	 * @param sensorName
	 * @return
	 */
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

} /* namespace twirre */

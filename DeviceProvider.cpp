 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
  */

#include "TwirreLink.h"

#include "DeviceProvider.h"


namespace twirre
{
	DeviceProvider::~DeviceProvider()
	{
		for(auto link : _links)
		{
			link->removeLink(this);
		}

		//notify all links that something has changed
		for(auto link : _links)
		{
			link->notifyChange();
		}
	}

	void DeviceProvider::doNotifyChange()
	{
		//set logger callbacks
		auto sensors = getSensors();
		for(auto& sensor : sensors)
		{
			sensor.second->setLoggerCallback([this](Sensor * sensor, std::map<std::string, Value*>& sensorValues){this->sensorLoggerCallback(sensor, sensorValues);});
		}
		auto actuators = getActuators();
		for(auto& actuator : actuators)
		{
			actuator.second->setActuateLoggerCallback([this](Actuator * actuator, std::map<std::string, Parameter*>& actuatorParams){this->actuatorLoggerCallback(actuator, actuatorParams);});
		}

		//notify all links that something has changed
		for(auto link : _links)
		{
			link->notifyChange();
		}
	}

	void DeviceProvider::addLink(TwirreLink * link)
	{
		_links.insert(link);
	}

	void DeviceProvider::removeLink(TwirreLink * link)
	{
		_links.erase(link);
	}

	void DeviceProvider::addLogger(TwirreLogger * log)
	{
		_loggers.insert(log);
	}

	void DeviceProvider::removeLogger(TwirreLogger * log)
	{
		_loggers.erase(log);
	}

	void DeviceProvider::sensorLoggerCallback(Sensor * sensor, std::map<std::string, Value*>& sensorValues)
	{
		for(auto & logger : _loggers)
		{
			logger->logSensorEvent(sensor, sensorValues);
		}
	}

	void DeviceProvider::actuatorLoggerCallback(Actuator * actuator, std::map<std::string, Parameter*>& actuatorParams)
	{
		for(auto & logger : _loggers)
		{
			logger->logActuatorEvent(actuator, actuatorParams);
		}
	}
}

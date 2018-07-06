/*
 * Twirre: architecture for autonomous UAVs using interchangeable commodity components
 *
 * Copyright© 2017 Centre of expertise in Computer Vision & Data Science, NHL Stenden University of applied sciences
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

	void DeviceProvider::addSensor(Sensor * s)
	{
		_sensors[s->getName()] = s;
		doNotifyChange();
	}

	void DeviceProvider::addActuator(Actuator * a)
	{
		_actuators[a->getName()] = a;
		doNotifyChange();
	}

	const std::map<std::string, Sensor*> & DeviceProvider::getSensors()
	{
		return _sensors;
	}

	const std::map<std::string, Actuator*> & DeviceProvider::getActuators()
	{
		return _actuators;
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

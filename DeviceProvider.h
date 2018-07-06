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

#ifndef DEVICEPROVIDER_H_
#define DEVICEPROVIDER_H_

#include <map>
#include <set>

#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "Core/Value.h"
#include "Logger/TwirreLogger.h"

namespace twirre
{
	class TwirreLink;

	/**
	 * A DeviceProvider is a collection of Actuators and Sensors which can be connected to a TwirreLink instance.
	 *
	 *
	 */
	class DeviceProvider
	{
		friend class TwirreLink;
	public:
		/**
		 * Default empty constructor
		 */
		DeviceProvider() {};
		virtual ~DeviceProvider();
		/**
		 * Add an Actuator to this provider
		 *
		 * @param a a pointer to the Actuator to be added
		 */
		virtual void addActuator(Actuator * a);
		/**
		 * Add a Sensor to this provider
		 *
		 * @param s a pointer to the Sensor to be added
		 */
		virtual void addSensor(Sensor * s);
		/**
		 * Get all available actuators
		 *
		 * @return A const reference to the internal map is returned. This map maps an actuator name to a pointer to the actual Actuator object.
		 */
		virtual const std::map<std::string, Actuator*> & getActuators();
		/**
		 * Get all available sensors
		 *
		 * @return A const reference to the internal map is returned. This map maps an sensor name to a pointer to the actual Sensor object.
		 */
		virtual const std::map<std::string, Sensor*> & getSensors();
	protected:
		/**
		 * Notifies all connected instances of TwirreLink that they should update, because a change (added/removed device) has occurred in this provider.
		 */
		void doNotifyChange();

		void addLogger(TwirreLogger * log);
		void removeLogger(TwirreLogger * log);
		std::map<std::string, Sensor*> _sensors;
		std::map<std::string, Actuator*> _actuators;
	private:
		void sensorLoggerCallback(Sensor * sensor, std::map<std::string, Value*>& sensorValues);
		void actuatorLoggerCallback(Actuator * actuators, std::map<std::string, Parameter*>& actuatorParameters);
		std::set<TwirreLogger *> _loggers;
		std::set<TwirreLink *> _links;
		void addLink(TwirreLink * link);
		void removeLink(TwirreLink * link);
	};

} /* namespace twirre */

#endif /* DEVICEPROVIDER_H_ */

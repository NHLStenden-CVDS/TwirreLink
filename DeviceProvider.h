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

	class DeviceProvider
	{
		friend class TwirreLink;
	public:
		DeviceProvider() {};
		virtual ~DeviceProvider();
		virtual const std::map<std::string, Actuator*> & getActuators() = 0;
		virtual const std::map<std::string, Sensor*> & getSensors() = 0;
	protected:
		void doNotifyChange();
		void addLogger(TwirreLogger * log);
		void removeLogger(TwirreLogger * log);
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

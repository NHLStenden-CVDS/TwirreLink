 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
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

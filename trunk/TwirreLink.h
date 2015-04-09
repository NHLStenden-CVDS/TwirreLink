/*
 * TwirreLink.h
 *
 *  Created on: Apr 9, 2015
 *      Author: root
 */

#ifndef TWIRRELINK_H_
#define TWIRRELINK_H_

#include <map>

#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "DeviceProvider.h"

namespace twirre
{
	class TwirreLink
	{
	public:
		TwirreLink();

		void addProvider(DeviceProvider& prov);

		bool haveSensor(const std::string & sensorName) const;
		bool haveActuator(const std::string & actuatorName) const;

		Sensor& getSensor(const std::string & sensorName);
		Actuator& getActuator(const std::string & actuatorName);

	private:
		std::map<std::string, Actuator*> _actuatorList;
		std::map<std::string, Sensor*> _sensorList;
	};

} /* namespace twirre */

#endif /* TWIRRELINK_H_ */

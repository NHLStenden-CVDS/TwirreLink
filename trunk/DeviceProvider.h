/*
 * DeviceProvider.h
 *
 *  Created on: Apr 9, 2015
 *      Author: root
 */

#ifndef DEVICEPROVIDER_H_
#define DEVICEPROVIDER_H_

#include <map>
#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "Core/Value.h"

namespace twirre
{
	class DeviceProvider
	{
	public:
		DeviceProvider() {};
		virtual ~DeviceProvider() {};
		virtual const std::map<std::string, Actuator*> & getActuators() = 0;
		virtual const std::map<std::string, Sensor*> & getSensors() = 0;
	};

} /* namespace twirre */

#endif /* DEVICEPROVIDER_H_ */

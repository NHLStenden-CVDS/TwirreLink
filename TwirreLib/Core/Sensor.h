/*
 * Sensor.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "Device.h"

#ifndef SENSOR_H_
#define SENSOR_H_

namespace twirre
{
	class Sensor : public Device
	{
		public:
			Sensor ();

			std::map <std::string, Value>	GetValue(void);
	};

} /* namespace twirre */

#endif /* SENSOR_H_ */

/*
 * TwirreLib.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "Serial/SerialRW.h"
#include "Support/Helper.h"

#ifndef TWIRRELIB_H_
#define TWIRRELIB_H_

namespace twirre
{
	class TwirreLib
	{
		public:
			TwirreLib();
			virtual 				~TwirreLib();
			std::string				init(char*);

			char					Ping(void);

			Sensor 					GetSensor(int);
			Actuator 				GetActuator(int);

			std::vector<Actuator> 	GetActuatorList(void);
			std::vector<Sensor> 	GetSensorList(void);

		private:
			std::vector<Actuator>	actuatorList;
			std::vector<Sensor>		sensorList;
			SerialRW				soiw;

			void 					initActuators(void);
	};

} /* namespace twirre */

#endif /* TWIRRELIB_H_ */

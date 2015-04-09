/*
 * Sensor.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef SERIALSENSOR_H_
#define SERIALSENSOR_H_

#include <vector>

#include "Core/Sensor.h"

namespace twirre
{
	class SerialSensor: public Sensor
	{
	public:
		SerialSensor(const uint8_t id, const std::string name, const std::string description, SerialRW & serialRW, const std::string valuesString);
		virtual ~SerialSensor();

		/*
		 * This class must never be passed by value
		 */
		//disable empty constructor
		SerialSensor() = delete;
		//disable copy
		SerialSensor(const SerialSensor&) = delete;
		//disable move
		SerialSensor(SerialSensor&&) = delete;
		//disable assignment
		SerialSensor& operator=(const SerialSensor&) = delete;
		//disable move assignment
		SerialSensor& operator=(SerialSensor&&) = delete;

		virtual std::map<std::string, Value*> Sense(const std::vector<std::string> &names) override;
	protected:
		SerialRW & _serial;
		void _ProcessValuesString(const std::string & s);
	};

} /* namespace twirre */

#endif /* SENSOR_H_ */

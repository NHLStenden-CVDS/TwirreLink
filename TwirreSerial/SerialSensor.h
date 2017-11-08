 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
  */

#ifndef SERIALSENSOR_H_
#define SERIALSENSOR_H_

#include <vector>
#include <mutex>

#include "../Core/Sensor.h"

namespace twirre
{
	class SerialSensor: public Sensor, public SerialDevice
	{
	public:
		SerialSensor(const int id, const std::string name, const std::string description, SerialRW & serialRW, const std::string valuesString, std::mutex & mutex);
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

		virtual std::map<std::string, Value*> Sense_impl(const std::vector<std::string> &names) override;
	protected:
		SerialRW & _serial;
		std::mutex & _serialMutex;
		void _ProcessValuesString(const std::string & s);
	};

} /* namespace twirre */

#endif /* SENSOR_H_ */

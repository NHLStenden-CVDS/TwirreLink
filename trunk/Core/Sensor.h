/*
 * Sensor.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef SENSOR_H_
#define SENSOR_H_

#include <vector>

#include "Device.h"

namespace twirre
{
	class Sensor: public Device
	{
	public:
		Sensor(const uint8_t id, const std::string name, const std::string description, SerialRW & serialRW, const std::string valuesString);
		virtual ~Sensor();

		/*
		 * This class must never be passed by value
		 */
		//disable empty constructor
		Sensor() = delete;
		//disable copy
		Sensor(const Sensor&) = delete;
		//disable move
		Sensor(Sensor&&) = delete;
		//disable assignment
		Sensor& operator=(const Sensor&) = delete;
		//disable move assignment
		Sensor& operator=(Sensor&&) = delete;

		std::map<std::string, Value*> SenseAll();

		Value & Sense(const std::string &name);
		std::map<std::string, Value*> Sense(const std::vector<std::string> &names);

		Value & operator[](const std::string &name);
		std::map<std::string, Value*> operator[](const std::vector<std::string> &names);

		virtual std::string ToString() override;
	protected:
		std::map<std::string, Value*> _valueList;
		void _ProcessValuesString(const std::string & s);
	};

} /* namespace twirre */

#endif /* SENSOR_H_ */

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
		Sensor(const std::string name, const std::string description);
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

		virtual std::map<std::string, Value*> SenseAll();

		virtual std::vector<std::string> getAvailableValues();
		virtual bool haveValue(std::string name);
		virtual bool haveValues(std::vector<std::string>);

		virtual Value & peekValue(std::string name);
		virtual std::map<std::string, Value*> peekValues(const std::vector<std::string> &names);

		virtual Value & Sense(const std::string &name);
		virtual std::map<std::string, Value*> Sense(const std::vector<std::string> &names) = 0;

		virtual Value & operator[](const std::string &name);
		virtual std::map<std::string, Value*> operator[](const std::vector<std::string> &names);

		virtual std::string ToString() override;
	protected:
		std::map<std::string, Value*> _valueList;
	};

} /* namespace twirre */

#endif /* SENSOR_H_ */

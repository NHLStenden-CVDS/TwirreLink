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

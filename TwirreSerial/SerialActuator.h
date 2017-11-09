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
#ifndef SERIALACTUATOR_H_
#define SERIALACTUATOR_H_

#include "../Core/Actuator.h"
#include "../TwirreSerial/SerialDevice.h"

namespace twirre
{
class SerialActuator: public Actuator, public SerialDevice
{
public:
	SerialActuator(const uint8_t id, const std::string name, const std::string description, SerialRW & serialRW, const std::string parametersString, std::mutex & mutex);
	~SerialActuator();

	/*
	 * This class must never be passed by value
	 */
	//disable empty constructor
	SerialActuator() = delete;
	//disable copy
	SerialActuator(const SerialActuator&) = delete;
	//disable move
	SerialActuator(SerialActuator&&) = delete;
	//disable assignment
	SerialActuator& operator=(const SerialActuator&) = delete;
	//disable move assignment
	SerialActuator& operator=(SerialActuator&&) = delete;

	virtual void ActuateImpl() override;

protected:
	void _ProcessParametersString(const std::string & s);
	SerialRW& _serial;
	std::mutex & _serialMutex;
};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

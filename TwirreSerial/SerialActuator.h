/*
 * Actuator.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
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
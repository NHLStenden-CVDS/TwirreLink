/*
 * Actuator.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include "Device.h"

namespace twirre
{
class Actuator: public Device
{
public:
	Actuator(const uint8_t id, const std::string name, const std::string description, SerialRW & serialRW, const std::string parametersString);
	virtual ~Actuator();

	/*
	 * This class must never be passed by value
	 */
	//disable empty constructor
	Actuator() = delete;
	//disable copy
	Actuator(const Actuator&) = delete;
	//disable move
	Actuator(Actuator&&) = delete;
	//disable assignment
	Actuator& operator=(const Actuator&) = delete;
	//disable move assignment
	Actuator& operator=(Actuator&&) = delete;

	Parameter & GetParameter(const std::string & name);
	Parameter & operator[] (const std::string & name);

	void Actuate();

protected:
	std::map<std::string, Parameter*> _parametersList;
	void _ProcessParametersString(const std::string & s);
};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

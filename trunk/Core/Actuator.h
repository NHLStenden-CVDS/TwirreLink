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
	Actuator(int id, string name, string description, SerialRW & serialRW);

	Value* GetParameter(void);
	void Actuate();

private:
	std::map<std::string, Value*> _parameters;
};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

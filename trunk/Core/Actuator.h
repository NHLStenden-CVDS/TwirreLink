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
	Actuator(const uint8_t id, const string name, const string description, SerialRW & serialRW, const string parametersString);

	Parameter* GetParameter(void);
	void Actuate();

protected:
	std::map<std::string, Parameter*> _parametersList;
	void _ProcessParametersString(const string & s);
};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

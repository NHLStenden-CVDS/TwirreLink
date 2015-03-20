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
	Actuator();

	std::map<std::string, Value> GetParameters(void);
	void Actuate(std::map<std::string, Value>);

private:
	std::map<std::string, Value> DefaultParameters;
};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

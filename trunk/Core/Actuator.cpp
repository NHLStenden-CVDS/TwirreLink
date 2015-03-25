/*
 * Actuator.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Actuator.h"

namespace twirre
{

Actuator::Actuator(int id, string name, string description, SerialRW & serialRW) : Device(id,name, description, serialRW)
{
	// TODO Auto-generated constructor stub

}

} /* namespace twirre */

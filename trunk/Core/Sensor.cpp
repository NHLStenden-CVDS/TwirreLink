/*
 * Sensor.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Sensor.h"

namespace twirre
{

Sensor::Sensor(int id, string name, string description, SerialRW & serialRW) : Device(id,name, description, serialRW)
{
	// TODO Auto-generated constructor stub

}

Value * Sensor::Sense(string valueName)
{
}

} /* namespace twirre */

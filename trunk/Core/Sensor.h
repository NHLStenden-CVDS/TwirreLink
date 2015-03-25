/*
 * Sensor.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef SENSOR_H_
#define SENSOR_H_

#include "Device.h"

namespace twirre
{
class Sensor: public Device
{
public:
	Sensor(const uint8_t id, const string name, const string description, SerialRW & serialRW, const string valuesString);


	Value * Sense(string valueName);
protected:
	map<string, Value*> _valueList;
	void _ProcessValuesString(const string & s);
};

} /* namespace twirre */

#endif /* SENSOR_H_ */

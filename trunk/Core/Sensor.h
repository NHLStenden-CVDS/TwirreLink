/*
 * Sensor.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef SENSOR_H_
#define SENSOR_H_

#include "Device.h"
#include <vector>

namespace twirre
{
class Sensor: public Device
{
public:
	Sensor(const uint8_t id, const string name, const string description, SerialRW & serialRW, const string valuesString);


	Value * Sense(const string &name);
	map<string, Value*> Sense(const vector<string> &names);

	Value* operator[](const string &name);
	map<string, Value*> operator[](const vector<string> &names);
protected:
	map<string, Value*> _valueList;
	void _ProcessValuesString(const string & s);
};

} /* namespace twirre */

#endif /* SENSOR_H_ */

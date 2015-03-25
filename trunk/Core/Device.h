/*
 * Device.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef DEVICE_H_
#define DEVICE_H_

#include <string>
#include <map>
#include "Value.h"
#include "SerialRW.h"

using namespace std;

namespace twirre
{
class Device
{
public:
	Device(const uint8_t id, const string name, const string description, SerialRW & serialRW);
	string ToString(void);
protected:
	const uint8_t _id;
	const string _name;
	const string _description;

	SerialRW & _serialRW;
};

} /* namespace twirre */

#endif /* DEVICE_H_ */

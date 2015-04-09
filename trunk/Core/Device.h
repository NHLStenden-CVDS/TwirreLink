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
#include <cstdint>

#include "Value.h"
#include "SerialRW.h"

namespace twirre
{
class Device
{
public:
	Device(const uint8_t id, const std::string name, const std::string description);
	virtual ~Device() {};

	virtual std::string ToString(void);
protected:
	const uint8_t _id;
	const std::string _name;
	const std::string _description;
};

} /* namespace twirre */

#endif /* DEVICE_H_ */

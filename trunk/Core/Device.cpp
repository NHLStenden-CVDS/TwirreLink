/*
 * Device.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Device.h"

namespace twirre
{

Device::Device(const uint8_t id, const string name, const string description, SerialRW & serialRW) : _id(id), _name(name), _description(description), _serialRW(serialRW)
{

}

std::string Device::ToString()
{
	char str[1024];
	sprintf(str, "%d", _id);
	std::string buf = str;
	buf.append(": ");
	buf.append(_name);
	buf.append(" -> ");
	buf.append(_description);

	return buf;
}
} /* namespace twirre */

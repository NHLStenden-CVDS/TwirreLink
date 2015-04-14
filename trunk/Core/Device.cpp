/*
 * Device.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Device.h"

using namespace std;

namespace twirre
{

Device::Device(const uint8_t id, const string name, const string description) : _id(id), _name(name), _description(description)
{

}

const std::string & Device::getName() const
{
	return _name;
}

const std::string & Device::getDescription() const
{
	return _description;
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

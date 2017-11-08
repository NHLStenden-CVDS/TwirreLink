 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
  */

#include "../Core/Device.h"

using namespace std;

namespace twirre
{

Device::Device(const string name, const string description) :_name(name), _description(description)
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
	std::string buf = "";
	buf.append(_name);
	buf.append(" -> ");
	buf.append(_description);

	return buf;
}
} /* namespace twirre */

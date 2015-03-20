/*
 * Device.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Device.h"

namespace twirre
{

Device::Device()
{
	// TODO Auto-generated constructor stub
}

std::string Device::ToString()
{
	char str[1024];
	sprintf(str, "%d", ID);
	std::string buf = str;
	buf.append(": ");
	buf.append(Name);
	buf.append(" -> ");
	buf.append(Description);

	return buf;
}
} /* namespace twirre */

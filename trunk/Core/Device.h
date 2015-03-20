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

using namespace std;

namespace twirre
{
class Device
{
public:
	Device();

	int ID;
	string Name;
	string Description;

	string ToString(void);

	map<string, Value> valueList;
};

} /* namespace twirre */

#endif /* DEVICE_H_ */

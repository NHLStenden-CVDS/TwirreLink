 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
  */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <string>
#include <map>
#include <cstdint>

#include "Value.h"

namespace twirre
{
class Device
{
public:
	Device(const std::string name, const std::string description);
	virtual ~Device() {};

	virtual const std::string & getName() const;
	virtual const std::string & getDescription() const;

	virtual std::string ToString(void);
protected:
	const std::string _name;
	const std::string _description;
};

} /* namespace twirre */

#endif /* DEVICE_H_ */

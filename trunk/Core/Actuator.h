/*
 * Actuator.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include "Device.h"

namespace twirre
{
class Actuator: public Device
{
public:
	Actuator(const uint8_t id, const std::string name, const std::string description);
	virtual ~Actuator();

	/*
	 * This class must never be passed by value
	 */
	//disable empty constructor
	Actuator() = delete;
	//disable copy
	Actuator(const Actuator&) = delete;
	//disable move
	Actuator(Actuator&&) = delete;
	//disable assignment
	Actuator& operator=(const Actuator&) = delete;
	//disable move assignment
	Actuator& operator=(Actuator&&) = delete;

	std::vector<std::string> getAvailableParameters();
	bool haveParameter(std::string name);
	bool haveParameters(std::vector<std::string> names);

	virtual Parameter & GetParameter(const std::string & name);
	virtual Parameter & operator[] (const std::string & name);

	virtual void Actuate() = 0;

protected:
	std::map<std::string, Parameter*> _parametersList;
};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

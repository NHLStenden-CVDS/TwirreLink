/*
 * Actuator.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include <vector>

#include "Support/Helper.h"

#include "Core/Actuator.h"

using namespace std;

namespace twirre
{

	Actuator::Actuator(const uint8_t id, const string name, const string description) :
			Device(id, name, description)
	{

	}

	Actuator::~Actuator()
	{
		for (auto & paramPair : _parametersList)
		{
			delete paramPair.second;
		}
	}

	Parameter & Actuator::GetParameter(const string & name)
	{
		if (_parametersList.find(name) == _parametersList.end())
		{
			return *ErrorValue::getInstance();
		}
		else
		{
			return *_parametersList.at(name);
		}
	}

	Parameter & Actuator::operator [](const string & name)
	{
		return GetParameter(name);
	}

} /* namespace twirre */

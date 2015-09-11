/*
 * Actuator.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include <vector>

#include "../Support/Helper.h"

#include "../Core/Actuator.h"

using namespace std;

namespace twirre
{

	Actuator::Actuator(const string name, const string description) :
			Device(name, description)
	{

	}

	Actuator::~Actuator()
	{

	}

	vector<string> Actuator::getAvailableParameters()
	{
		vector<string> ret;

		for (const auto& pair : _parametersList)
		{
			ret.push_back(pair.first);
		}

		return ret;
	}

	bool Actuator::haveParameter(string name)
	{
		return haveParameters({ name });
	}

	bool Actuator::haveParameters(vector<string> names)
	{
		for (const auto& name : names)
		{
			if (_parametersList.find(name) == _parametersList.end()) return false;
		}

		return true;
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

	void Actuator::Actuate()
	{
		//lock the owned_mutex
		_actuateMutex.lock();

		//call actuation implementation
		ActuateImpl();

		//unlock the owned mutex
		_actuateMutex.unlock();
	}

	void Actuator::registerParameter(Parameter* parm)
	{
		_parametersList[parm->getName()] = parm;
	}

	void Actuator::registerParameters(std::vector<Parameter *> parms)
	{
		for(auto parm : parms)
		{
			registerParameter(parm);
		}
	}

} /* namespace twirre */

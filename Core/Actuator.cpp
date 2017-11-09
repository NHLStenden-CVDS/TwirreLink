/*
 * Twirre: architecture for autonomous UAVs using interchangeable commodity components
 *
 * Copyright© 2017 Centre of expertise in Computer Vision & Data Science, NHL Stenden University of applied sciences
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
		clearActuateLoggerCallback();
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

	std::map<std::string, Parameter*> Actuator::GetParameters(const std::vector<std::string> & names)
	{
		std::map<string, Parameter*> returnvalues;

		for(const auto& name : names)
		{
			//skip duplicate names
			if(returnvalues.find(name) == returnvalues.end())
			{
				if(_parametersList.find(name) == _parametersList.end())
				{
					returnvalues[name] = ErrorValue::getInstance();
				}
				else
				{
					returnvalues[name] = _parametersList.at(name);
				}
			}
		}

		return returnvalues;
	}

	void Actuator::Actuate()
	{
		//lock the owned_mutex
		_actuateMutex.lock();

		//log
		if(_actuateLoggerSet)
		{
			logActuation();
		}

		//call actuation implementation
		ActuateImpl();

		//clear modify flag
		for(auto param : _parametersList)
		{
			if(param.second->isModified())
			{
				param.second->resetModified();
			}
		}

		//unlock the owned mutex
		_actuateMutex.unlock();
	}

	void Actuator::logActuation()
	{
		std::map<std::string, Parameter*> actuatedParams;
		for(auto param : _parametersList)
		{
			if(param.second->isModified())
			{
				actuatedParams.insert(param);
			}
		}
		_actuateLoggerCallback(this, actuatedParams);
	}

	void Actuator::registerParameter(Parameter* parm)
	{
		parm->setActuatorMutex(&_actuateMutex);
		_parametersList[parm->getName()] = parm;
	}

	void Actuator::registerParameters(std::vector<Parameter *> parms)
	{
		for(auto parm : parms)
		{
			registerParameter(parm);
		}
	}

	void Actuator::clearActuateLoggerCallback()
	{
		_actuateLoggerCallback = [](Actuator *, std::map<std::string, Parameter*>&){};
		_actuateLoggerSet = false;
	}

	void Actuator::setActuateLoggerCallback(std::function<void(Actuator *, std::map<std::string, Parameter*>&)> cbfn)
	{
		_actuateLoggerCallback = cbfn;
		_actuateLoggerSet = true;
	}

} /* namespace twirre */

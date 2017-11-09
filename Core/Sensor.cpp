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
#include <algorithm>
#include "../Support/Helper.h"

#include "../Core/Sensor.h"

using namespace std;

namespace twirre
{

	Sensor::Sensor(const string name, const string description) :
			Device(name, description)
	{
		clearLoggerCallback();
	}

	Sensor::~Sensor()
	{

	}

	map<string, Value*> Sensor::SenseAll()
	{
		vector<string> names;
		for(const auto & vals : _valueList)
		{
			names.push_back(vals.first);
		}
		return Sense(names);
	}

	vector<string> Sensor::getAvailableValues()
	{
		vector<string> ret;

		for(const auto& pair : _valueList)
		{
			ret.push_back(pair.first);
		}

		return ret;
	}

	bool Sensor::haveValue(string name)
	{
		return haveValues({name});
	}

	bool Sensor::haveValues(vector<string> names)
	{
		for(const auto& name : names)
		{
			if(_valueList.find(name) == _valueList.end())
				return false;
		}

		return true;
	}

	Value& Sensor::peekValue(std::string name)
	{
		if(_valueList.find(name) == _valueList.end())
			return *ErrorValue::getInstance();

		return *_valueList.at(name);
	}

	std::map<std::string, Value*> Sensor::peekValues(const std::vector<std::string>& names)
	{
		std::map<string, Value*> returnvalues;

		for(const auto& name : names)
		{
			//skip duplicate names
			if(returnvalues.find(name) == returnvalues.end())
			{
				if(_valueList.find(name) == _valueList.end())
				{
					returnvalues[name] = ErrorValue::getInstance();
				}
				else
				{
					returnvalues[name] = _valueList.at(name);
				}
			}
		}

		return returnvalues;
	}

	void Sensor::onSense(const std::vector<std::string> &names)
	{
		//default: do nothing
	}

	Value & Sensor::Sense(const string &valueName)
	{

		return *Sense(vector<string>{ valueName })[valueName];
	}

	std::map<std::string, Value*> Sensor::Sense(const std::vector<std::string> &names)
	{
		onSense(names);

		auto vals = Sense_impl(names);

		//call logger if required
		_loggerCallback(this, vals);

		return vals;
	}

	std::map<std::string, Value*> Sensor::Sense_impl(const std::vector<std::string> &names)
	{
		std::map<std::string, Value*> returnValues;

		for(auto & name : names)
		{
			//Add each requested value if it exists, and hasn't been added yet
			if(_valueList.find(name) != _valueList.end())
			{
				if(returnValues.find(name) == returnValues.end())
				{
					returnValues[name] = _valueList[name];
				}
			}
			else
			{
				//The requested value does not exist, add a pointer to ErrorValue instead
				returnValues[name] = ErrorValue::getInstance();
			}
		}

		return returnValues;
	}

	Value & Sensor::operator[](const string &name)
	{
		return Sense(name);
	}

	map<string, Value*> Sensor::operator[](const vector<string> &names)
	{
		return Sense(names);
	}

	string Sensor::ToString()
	{
		string s = "";
		s.reserve(2048);
		s += _name + ": " + _description + "\n";
		for(const auto& pair : _valueList)
		{
			s += "\t";
			s += pair.second->getName();
			s += "\n";
		}

		return s;
	}

	void Sensor::registerValue(Value* val)
	{
		_valueList[val->getName()] = val;
	}

	void Sensor::registerValues(std::vector<Value*> vals)
	{
		for(auto val : vals)
		{
			registerValue(val);
		}
	}

	void Sensor::clearLoggerCallback()
	{
		_loggerCallback = [](Sensor *, std::map<std::string, Value*>&){};
	}

	void Sensor::setLoggerCallback(std::function<void(Sensor *, std::map<std::string, Value*>&)> cbfn)
	{
		_loggerCallback = cbfn;
	}

} /* namespace twirre */



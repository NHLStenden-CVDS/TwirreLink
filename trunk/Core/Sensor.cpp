/*
 * Sensor.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */


#include <vector>
#include <algorithm>
#include "Helper.h"

#include "Core/Sensor.h"

using namespace std;

namespace twirre
{

	Sensor::Sensor(const uint8_t id, const string name, const string description) :
			Device(id, name, description)
	{
	}

	Sensor::~Sensor()
	{
		for(const auto & valuePair : _valueList)
		{
			delete valuePair.second;
		}
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


	Value & Sensor::Sense(const string &valueName)
	{

		return *Sense(vector<string>{ valueName })[valueName];
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
			s += to_string(pair.second->getId());
			s += " ";
			s += pair.second->getName();
			s += "\n";
		}

		return s;
	}

} /* namespace twirre */


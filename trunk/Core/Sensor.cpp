/*
 * Sensor.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Sensor.h"
#include <vector>
#include <algorithm>
#include "Helper.h"
#include "TwirreSerial.h"

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

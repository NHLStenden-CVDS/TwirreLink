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
#include "TwirreLib.h"

namespace twirre
{

	Sensor::Sensor(const uint8_t id, const string name, const string description, SerialRW & serialRW, const string valuesString) :
			Device(id, name, description, serialRW)
	{
		_ProcessValuesString(valuesString);
	}

	Value * Sensor::Sense(const string &valueName)
	{

		return Sense(vector<string>{ valueName })[valueName];
	}

	Value* Sensor::operator[](const string &name)
	{
		return Sense(name);
	}
	/**
	 * Returns the updated requested values
	 *
	 * Duplicate names will be ignored
	 * If a given value name is invalid, ErrorValue will be returned for that name
	 * If the serial communication failed the old values will be returned
	 */
	map<string, Value*> Sensor::Sense(const vector<string> &names)
	{
		map<string, Value*> valuesMap;
		vector<Value*> values;

		for (const auto& str : names)
		{
			if (!(_valueList.find(str) == _valueList.end()))
			{
				Value* val = _valueList.at(str);
				//check for duplicate names
				if(end(values) == std::find(begin(values), end(values), val))
				{
					values.push_back(val);
					valuesMap[str] = val;
				}
			}
			else
			{
				valuesMap[str] = ErrorValue::getInstance();
			}
		}

		unsigned char* message = reinterpret_cast<unsigned char*>(malloc(values.size() + 4));
		message[0] = 'S';
		message[1] = _id;
		*reinterpret_cast<uint16_t*>(&message[2]) = static_cast<uint16_t>(values.size());

		int nextId = 4;
		for (const auto& val : values)
		{
			message[nextId++] = val->_id;
		}

		_serialRW.writeBytes(message, values.size() + 4);

		delete message;

		if (TwirreLib::CheckOk(_serialRW))
		{
			for (const auto& val : values)
			{
				val->UpdateFromSerial();
			}
		}

		return valuesMap;
	}

	map<string, Value*> Sensor::operator[](const vector<string> &names)
	{
		return Sense(names);
	}

	void Sensor::_ProcessValuesString(const string & s)
	{
		std::vector<std::string> valueStrings;
		Helper::split(s, ',', valueStrings);

		for (int i = 0; i < valueStrings.size(); i++)
		{
			std::vector<std::string> nameAndType;
			Helper::split(valueStrings[i], '=', nameAndType);

			Value* value = ErrorValue::getInstance();
			if (!nameAndType[1].compare("UI8")) value = new ValueImpl<uint8_t>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("I8")) value = new ValueImpl<int8_t>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("UI16")) value = new ValueImpl<uint16_t>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("I16")) value = new ValueImpl<int16_t>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("UI32")) value = new ValueImpl<uint32_t>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("I32")) value = new ValueImpl<int32_t>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("UI64")) value = new ValueImpl<uint64_t>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("I64")) value = new ValueImpl<int64_t>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("F")) value = new ValueImpl<float>(i, nameAndType[0], 0, &_serialRW);
			if (!nameAndType[1].compare("D")) value = new ValueImpl<double>(i, nameAndType[0], 0, &_serialRW);

			_valueList.insert(pair<string, Value*>(nameAndType[0], value));
		}
	}

	string Sensor::ToString()
	{
		string s = "";
		s.reserve(2048);
		s += _name + ": " + _description + "\n";
		for(const auto& pair : _valueList)
		{
			s += "\t";
			s += to_string(pair.second->_id);
			s += " ";
			s += pair.second->_name;
			s += "\n";
		}

		return s;
	}

} /* namespace twirre */

/*
 * Sensor.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Sensor.h"
#include <vector>
#include "Helper.h"
#include "TwirreLib.h"

namespace twirre
{

Sensor::Sensor(const uint8_t id, const string name, const string description, SerialRW & serialRW, const string valuesString) : Device(id,name, description, serialRW)
{
	_ProcessValuesString(valuesString);
}

Value * Sensor::Sense(const string &valueName)
{

	if(_valueList.find(valueName) == _valueList.end())
	{
		return nullptr;
	}


	Value * value = _valueList.at(valueName);

#pragma pack(push,1)
	struct
	{
		char requestType;
		uint8_t sensorId;
		uint16_t nrOfRequestedValues;
		uint8_t requestedValueId;
	} requestPackage = {'S', _id, 1, value->getId()};
#pragma pack(pop)

	_serialRW.Write(requestPackage);

	if(!TwirreLib::CheckOk(_serialRW))
	{
		return nullptr;
	}

	value->UpdateFromSerial();

	return value;
}

Value* Sensor::operator[](const string &name)
{
	return Sense(name);
}

map<string, Value*> Sensor::Sense(const vector<string> &names)
{
	map<string, Value*> valuesMap;
	vector<Value*> values;
	for(const auto& str : names)
	{
		if(!(_valueList.find(str) == _valueList.end()))
		{
			Value* val = _valueList.at(str);
			values.push_back(val);
			valuesMap[str] = val;
		}
	}

/*
	unsigned char buf[100];
	buf[0] = 'S';
	buf[1] = 2;
	buf[2] = 3;
	buf[3] = 0;
	buf[4] = values.at(0)->_id;
	buf[5] = values.at(1)->_id;
	buf[6] = values.at(2)->_id;
	*/

#pragma pack(push,1)
	struct
	{
		char requestType;
		uint8_t sensorId;
		uint16_t nrOfRequestedValues;
	} requestPackage = {'S', _id, static_cast<uint16_t>(values.size())};
#pragma pack(pop)

	_serialRW.Write(requestPackage);
	for(const auto& val : values)
	{
		_serialRW.Write(val->getId());
	}

	//_serialRW.writeBytes(buf, 7);

	if(!TwirreLib::CheckOk(_serialRW))
	{
		return map<string, Value*>();
	}

	for(const auto& val : values)
	{
		val->UpdateFromSerial();
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

		//Value value(i, nameAndType[0], nameAndType[1]);
		Value* value = nullptr;
		if(!nameAndType[1].compare("UI8"))
			value = new ValueImpl<uint8_t>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("I8"))
					value = new ValueImpl<int8_t>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("UI16"))
					value = new ValueImpl<uint16_t>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("I16"))
					value = new ValueImpl<int16_t>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("UI32"))
					value = new ValueImpl<uint32_t>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("I32"))
					value = new ValueImpl<int32_t>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("UI64"))
					value = new ValueImpl<uint64_t>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("I64"))
					value = new ValueImpl<int64_t>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("F"))
					value = new ValueImpl<float>(i, nameAndType[0], 0, _serialRW);
		if(!nameAndType[1].compare("D"))
					value = new ValueImpl<double>(i, nameAndType[0], 0, _serialRW);

		_valueList.insert(pair<string, Value*>(nameAndType[0], value));
	}
}

} /* namespace twirre */

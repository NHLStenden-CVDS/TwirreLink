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
#include "../TwirreSerial/SerialValue.h"
#include "../TwirreSerial/TwirreSerial.h"

#include "../TwirreSerial/SerialSensor.h"

using namespace std;

namespace twirre
{

	SerialSensor::SerialSensor(const int id, const string name, const string description, SerialRW & serialRW, const string valuesString, std::mutex & mutex) :
			Sensor(name, description), SerialDevice(id), _serial(serialRW), _serialMutex(mutex)
	{
		_ProcessValuesString(valuesString);
	}

	SerialSensor::~SerialSensor()
	{
		for(auto & pair : _valueList)
		{
			delete pair.second;
		}
	}

	/**
	 * Returns the updated requested values
	 *
	 * Duplicate names will be ignored
	 * If a given value name is invalid, ErrorValue will be returned for that name
	 * If the serial communication failed the old values will be returned
	 */
	map<string, Value*> SerialSensor::Sense_impl(const vector<string> &names)
	{
		map<string, Value*> valuesMap;
		vector<Value*> values;

		for (const auto& str : names)
		{
			if (!(_valueList.find(str) == _valueList.end()))
			{
				Value* val = _valueList.at(str);
				//check for duplicate names
				if((end(values) == std::find(begin(values), end(values), val)) && val->isValid())
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
			message[nextId++] = dynamic_cast<SerialValue *>(val)->getID();
		}

		//serial comm block
		{
			std::lock_guard<std::mutex> lock(_serialMutex);

			_serial.writeBytes(message, values.size() + 4);

			if (TwirreSerial::CheckOk(_serial))
			{
				for (const auto& val : values)
				{
					dynamic_cast<SerialValue*>(val)->updateFromSerial();
				}
			}
		}

		delete message;

		return valuesMap;
	}

	void SerialSensor::_ProcessValuesString(const string & s)
	{
		std::vector<std::string> valueStrings;
		Helper::split(s, ',', valueStrings);

		for (size_t i = 0; i < valueStrings.size(); i++)
		{
			std::vector<std::string> nameAndType;
			Helper::split(valueStrings[i], '=', nameAndType);

			Value* value = ErrorValue::getInstance();
			if (!nameAndType[1].compare("UI8")) value = new SerialValueImpl<uint8_t>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("I8")) value = new SerialValueImpl<int8_t>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("UI16")) value = new SerialValueImpl<uint16_t>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("I16")) value = new SerialValueImpl<int16_t>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("UI32")) value = new SerialValueImpl<uint32_t>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("I32")) value = new SerialValueImpl<int32_t>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("UI64")) value = new SerialValueImpl<uint64_t>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("I64")) value = new SerialValueImpl<int64_t>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("F")) value = new SerialValueImpl<float>(i, nameAndType[0], 0, _serial);
			if (!nameAndType[1].compare("D")) value = new SerialValueImpl<double>(i, nameAndType[0], 0, _serial);

			if (!nameAndType[1].compare("A:UI8")) value = new SerialArrayValue<uint8_t>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:I8")) value = new SerialArrayValue<int8_t>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:UI16")) value = new SerialArrayValue<uint16_t>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:I16")) value = new SerialArrayValue<int16_t>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:UI32")) value = new SerialArrayValue<uint32_t>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:I32")) value = new SerialArrayValue<int32_t>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:UI64")) value = new SerialArrayValue<uint64_t>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:I64")) value = new SerialArrayValue<int64_t>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:F")) value = new SerialArrayValue<float>(i, nameAndType[0], _serial);
			if (!nameAndType[1].compare("A:D")) value = new SerialArrayValue<double>(i, nameAndType[0], _serial);

			registerValue(value);
		}
	}
} /* namespace twirre */

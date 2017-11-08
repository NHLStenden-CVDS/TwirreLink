 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
  */
#include <vector>
#include <mutex>

#include "TwirreSerial.h"
#include "SerialValue.h"
#include "../Support/Helper.h"

#include "../TwirreSerial/SerialActuator.h"

using namespace std;

namespace twirre
{

	SerialActuator::SerialActuator(const uint8_t id, const string name, const string description, SerialRW & serialRW, const string parametersString, std::mutex & mutex) :
			Actuator(name, description), SerialDevice(id), _serial(serialRW), _serialMutex(mutex)
	{
		_ProcessParametersString(parametersString);
	}

	SerialActuator::~SerialActuator()
	{
		for(auto & pair : _parametersList)
		{
			delete pair.second;
		}
	}

	void SerialActuator::ActuateImpl()
	{
		vector<unsigned char> message;
		vector<Parameter *> paramsToSet;

		//find all modified values
		for (auto & pair : _parametersList)
		{
			if (pair.second->isModified())
			{
				paramsToSet.push_back(pair.second);
			}
		}

		//set message header
		message.push_back('A');
		message.push_back(_id);

		//reserve space for payload size (we will only know this after all parameters have been added)
		message.push_back(0);
		message.push_back(0);

		//add all parameters
		for (const auto param : paramsToSet)
		{
			SerialValue* v = dynamic_cast<SerialValue*>(param);
			v->addToMessage(message);
		}

		//overwrite payload size with new value
		*reinterpret_cast<uint16_t*>(&(message.data()[2])) = static_cast<uint16_t>(message.size() - 4);

		//serial comm block
		{
			std::lock_guard<std::mutex> lock(_serialMutex);

			//transmit message
			_serial.writeBytes(message.data(), message.size());

			//check response
			TwirreSerial::CheckOk(_serial);
		}
	}

	void SerialActuator::_ProcessParametersString(const string & s)
	{
		std::vector<std::string> valueStrings;
		Helper::split(s, ',', valueStrings);

		for (size_t i = 0; i < valueStrings.size(); i++)
		{
			std::vector<std::string> nameAndType;
			Helper::split(valueStrings[i], '=', nameAndType);

			//Value value(i, nameAndType[0], nameAndType[1]);
			Parameter* value = ErrorValue::getInstance();
			if (!nameAndType[1].compare("UI8")) value = new SerialValueImpl<uint8_t>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("I8")) value = new SerialValueImpl<int8_t>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("UI16")) value = new SerialValueImpl<uint16_t>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("I16")) value = new SerialValueImpl<int16_t>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("UI32")) value = new SerialValueImpl<uint32_t>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("I32")) value = new SerialValueImpl<int32_t>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("UI64")) value = new SerialValueImpl<uint64_t>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("I64")) value = new SerialValueImpl<int64_t>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("F")) value = new SerialValueImpl<float>(i, nameAndType[0], 0, _serial, &_actuateMutex);
			if (!nameAndType[1].compare("D")) value = new SerialValueImpl<double>(i, nameAndType[0], 0, _serial, &_actuateMutex);

			if (!nameAndType[1].compare("A:UI8")) value = new SerialArrayValue<uint8_t>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:I8")) value = new SerialArrayValue<int8_t>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:UI16")) value = new SerialArrayValue<uint16_t>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:I16")) value = new SerialArrayValue<int16_t>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:UI32")) value = new SerialArrayValue<uint32_t>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:I32")) value = new SerialArrayValue<int32_t>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:UI64")) value = new SerialArrayValue<uint64_t>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:I64")) value = new SerialArrayValue<int64_t>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:F")) value = new SerialArrayValue<float>(i, nameAndType[0], _serial, &_actuateMutex);
			if (!nameAndType[1].compare("A:D")) value = new SerialArrayValue<double>(i, nameAndType[0], _serial, &_actuateMutex);

			registerParameter(value);
		}
	}

} /* namespace twirre */

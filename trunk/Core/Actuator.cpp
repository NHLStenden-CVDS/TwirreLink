/*
 * Actuator.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "../Core/Actuator.h"
#include <vector>
#include "TwirreLib.h"
#include "Helper.h"


namespace twirre
{

Actuator::Actuator(const uint8_t id, const string name, const string description, SerialRW & serialRW, const string parametersString) : Device(id,name, description, serialRW)
{
	_ProcessParametersString(parametersString);
}

void Actuator::Actuate()
{
	vector<unsigned char> message;
	vector<Parameter *> paramsToSet;

	//find all modified values
	for(auto & pair : _parametersList)
	{
		if(pair.second->_modified)
		{
			pair.second->resetModified();
			paramsToSet.push_back(pair.second);
		}
	}

	//set message header
	message.push_back('A');
	message.push_back(_id);

	//set amount of updated parameters
	uint16_t count = static_cast<uint16_t>(paramsToSet.size());
	unsigned char * count_b = reinterpret_cast<unsigned char *>(&count);
	message.push_back(count_b[0]);
	message.push_back(count_b[1]);

	//add all parameters
	for(const auto & param : paramsToSet)
	{
		message.push_back(param->_id);
		param->addValue(message);
	}

	//transmit message
	_serialRW.writeBytes(message.data(), message.size());

	//check response
	TwirreLib::CheckOk(_serialRW);
}

void Actuator::_ProcessParametersString(const string & s)
{
	std::vector<std::string> valueStrings;
	Helper::split(s, ',', valueStrings);

	for (int i = 0; i < valueStrings.size(); i++)
	{
		std::vector<std::string> nameAndType;
		Helper::split(valueStrings[i], '=', nameAndType);

		//Value value(i, nameAndType[0], nameAndType[1]);
		Parameter* value = ErrorValue::getInstance();
		if(!nameAndType[1].compare("UI8"))
			value = new ValueImpl<uint8_t>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("I8"))
			value = new ValueImpl<int8_t>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("UI16"))
			value = new ValueImpl<uint16_t>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("I16"))
			value = new ValueImpl<int16_t>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("UI32"))
			value = new ValueImpl<uint32_t>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("I32"))
			value = new ValueImpl<int32_t>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("UI64"))
			value = new ValueImpl<uint64_t>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("I64"))
			value = new ValueImpl<int64_t>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("F"))
			value = new ValueImpl<float>(i, nameAndType[0], 0, &_serialRW);
		if(!nameAndType[1].compare("D"))
			value = new ValueImpl<double>(i, nameAndType[0], 0, &_serialRW);

		_parametersList.insert(pair<string, Parameter*>(nameAndType[0], value));
	}
}

} /* namespace twirre */

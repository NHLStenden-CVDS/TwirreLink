/*
 * TwirreLib.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "TwirreLib.h"

using namespace twirre;

int main()
{
	TwirreLib twirre;
	twirre.Init("/dev/ttyACM0");
	return 0;
}

namespace twirre
{

TwirreLib::TwirreLib()
{
}

bool TwirreLib::Init(const char * device)
{
	//Initialize serial port
	if (_soiw.Initialize(device, 115200) != -1)
	{
		//Wait for arduino to initialize the connection
		sleep(1);
		cout << "Serial port initialized." << endl;

		//Flush file descriptor (discard read/write data)
		_soiw.flush();

		//Initialize sensors and actuators
		if(_InitActuators() && _InitSensors())
		{
			cout << "Twirre Library ready to use!" << endl;
			return true;
		}
	}
	cout << "Twirre Library NOT initialized :(" << endl;
	return false;
}

bool TwirreLib::_InitActuators()
{
#pragma pack(push,1)
	struct ia{
		char i = 'I';
		char a = 'A';
	};
#pragma pack(pop)
	ia m;
	_soiw.Write<ia>(m);
	sleep(1);
	std::string s;
	if(_soiw.readString(s))
	{
		if(_ProcessInitString<Actuator>(s, _actuatorList))
		{
			cout << "Actuators initialized. Number of actuators: "
				<< _actuatorList.size() << endl;
			return true;
		}
	}
	else
	{
		cout << "Actuators NOT initialized. Error reading initialization string.";
	}
	return false;
}

bool TwirreLib::_InitSensors()
{
	struct is{
		char i = 'I';
		char s = 'S';
	};
	is m;
	_soiw.Write<is>(m);
	sleep(1);
	std::string s;
	if(_soiw.readString(s))
	{
		if(_ProcessInitString<Sensor>(s, _sensorList))
		{
			cout << "Sensors initialized. Number of sensors: " << _sensorList.size()
				<< endl;
			return true;
		}
	}
	else
	{
		cout << "Sensors NOT initialized. Error reading initialization string.";
	}
	return false;
}

template<typename T> bool TwirreLib::_ProcessInitString(string & s, map<string, T> &deviceList)
{
	if (s.length() > 1)
	{
		char responseCode = s[0];
		std::string payloadString = s.substr(1);
		if (responseCode == 'O')
		{

			std::vector<std::string> deviceStrings;

			// Split all devices into a vector of device strings
			Helper::split(payloadString, ';', deviceStrings);

			for (int i = 0; i < deviceStrings.size(); ++i)
			{
				std::vector<std::string> deviceInformation;
				Helper::split(deviceStrings[i], '|', deviceInformation);


				// input: id, name, description, serial interface
				T device(i, deviceInformation[0], deviceInformation[1], _soiw);

				device.valueList = _ProcessValuesString(deviceInformation[2]);

				cout << device.ToString() << endl;

				deviceList.insert(pair<string, T>(deviceInformation[0], device));
			}
		}
		else if (responseCode == 'E')
		{
			cout << "Error received from Arduino: " << payloadString << endl;
			return false;
		}
		else
		{
			cout << "NOOOOOOOOOOOOOOOOOOO!" << endl;
			// something broke quite hard
			return false;
		}
	}
	return true;
}

std::map<string, Value*> TwirreLib::_ProcessValuesString(string & s)
{
	std::vector<std::string> valueStrings;
	Helper::split(s, ',', valueStrings);

	map<string, Value*> values;

	for (int i = 0; i < valueStrings.size(); i++)
	{
		std::vector<std::string> nameAndType;
		Helper::split(valueStrings[i], '=', nameAndType);

		//Value value(i, nameAndType[0], nameAndType[1]);
		Value* value = nullptr;
		if(!nameAndType[1].compare("UI8"))
			value = new ValueImpl<uint8_t>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("I8"))
					value = new ValueImpl<int8_t>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("UI16"))
					value = new ValueImpl<uint16_t>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("I16"))
					value = new ValueImpl<int16_t>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("UI32"))
					value = new ValueImpl<uint32_t>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("I32"))
					value = new ValueImpl<int32_t>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("UI64"))
					value = new ValueImpl<uint64_t>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("I64"))
					value = new ValueImpl<int64_t>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("F"))
					value = new ValueImpl<float>(i, nameAndType[0], 0, _soiw);
		if(!nameAndType[1].compare("D"))
					value = new ValueImpl<double>(i, nameAndType[0], 0, _soiw);

		values.insert(pair<string, Value*>(nameAndType[0], value));
	}

	return values;
}
//
//Value* TwirreLib::Sense(string sensorName, string valueName)
//{
//	if(_sensorList.find(sensorName) == _sensorList.end()){
//		throw new runtime_error("Sense: sensor name not in the list");
//	}
//
//	Sensor *sensor = &_sensorList.at(sensorName);
//	if(sensor->valueList.find(valueName) == sensor->valueList.end()){
//		throw new runtime_error("Sense: value name not in the list");
//	}
//	Value *value = sensor->valueList.at(valueName);
//
//	//Write sense request
//	MessageHeader mh = {'S', sensor->_id, 1};
//	_soiw.Write<MessageHeader>(mh);
//	unsigned char valueID = value->id;
//	_soiw.Write<unsigned char>(valueID);
//
//	//Receive response
//	char opcode;
//	if(_soiw.Read<char>(opcode))
//	{
//		if(opcode =='O'){
//			int bufferSize = value->GetSize();
//			unsigned char *buffer = new unsigned char[bufferSize];
//			_soiw.readNBytes(buffer, bufferSize);
//			value->SetBuffer(buffer);
//			delete buffer;
//		}
//		else
//		{
//			throw new runtime_error("error from arduino while reading sensor value.");
//		}
//	}
//	else
//	{
//		throw new runtime_error("Timeout in Sense(). No response from arduino.");
//	}
//
//	return value;
//}

Actuator& TwirreLib::GetActuator(string actuatorName)
{
	if(_actuatorList.find(actuatorName) == _actuatorList.end()){
		throw new runtime_error("GetActuator: actuator id out of bounds");
	}
	return _actuatorList.at(actuatorName);
}

Sensor& TwirreLib::GetSensor(string sensorName)
{
	if(_sensorList.find(sensorName) == _sensorList.end()){
		throw new runtime_error("GetSensor: sensor id out of bounds");
	}
	return _sensorList.at(sensorName);
}

bool TwirreLib::Ping()
{
	TwirreLib::_soiw.Write('P');
	char p;
	_soiw.Read<char>(p);
	return p == 'P';
}

TwirreLib::~TwirreLib()
{
}

} /* namespace twirre */

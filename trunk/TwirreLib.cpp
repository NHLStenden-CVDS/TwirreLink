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

bool TwirreLib::Init(char * device)
{
	//Initialize serial port
	if (TwirreLib::_soiw.Initialize(device, 115200) != -1)
	{
		//Wait for arduino to initialize the connection
		sleep(1);
		cout << "Serial port initialized." << endl;

		//Flush file descriptor (discard read/write data)
		TwirreLib::_soiw.flush();

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
	struct ia{
		char i = 'I';
		char a = 'A';
	};
	ia m;
	TwirreLib::_soiw.Write<ia>(m);
	sleep(1);
	std::string s;
	if(TwirreLib::_soiw.readString(s))
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
	TwirreLib::_soiw.Write<is>(m);
	sleep(1);
	std::string s;
	if(TwirreLib::_soiw.readString(s))
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

				T device;

				device.ID = i;
				device.Name = deviceInformation[0];
				device.Description = deviceInformation[1];

				device.valueList = _ProcessValuesString(deviceInformation[2]);

				cout << "Device " << device.ID << ": " << device.Name << ". "
						<< device.Description << endl;

				deviceList.insert(pair<string, T>(device.Name, device));
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

std::map<string, Value> TwirreLib::_ProcessValuesString(string & s)
{
	std::vector<std::string> valueStrings;
	Helper::split(s, ',', valueStrings);

	map<string, Value> values;

	for (int i = 0; i < valueStrings.size(); i++)
	{
		std::vector<std::string> nameAndType;
		Helper::split(valueStrings[i], '=', nameAndType);

		Value value(i, nameAndType[0], nameAndType[1]);

		values.insert(pair<string, Value>(nameAndType[0], value));
	}

	return values;
}

Value TwirreLib::Sense(string sensorName, string valueName)
{
	if(_sensorList.find(sensorName) == _sensorList.end()){
		throw new runtime_error("Sense: sensor name not in the list");
	}
	Sensor *sensor = &_sensorList.at(sensorName);
	if(sensor->valueList.find(valueName) == sensor->valueList.end()){
		throw new runtime_error("Sense: value name not in the list");
	}
	Value *value = &sensor->valueList.at(valueName);

	//Write sense request
	MessageHeader mh;
	mh.opcode = 'S';
	mh.targetID = sensor->ID;
	mh.payloadSize = 1;
	_soiw.Write<MessageHeader>(mh);
	unsigned char valueID = value->ID;
	_soiw.Write<unsigned char>(valueID);

	//Wait for processing response
	usleep(50);

	//Receive response
	unsigned char opcode;
	if(_soiw.Read<unsigned char>(opcode))
	{
		if(opcode =='O'){
			int bufferSize = value->GetSize();
			unsigned char *buffer = new unsigned char[bufferSize];
			_soiw.readNBytes(buffer, bufferSize);
			value->SetBuffer(buffer);
			delete buffer;
		}
		else
		{
			throw new runtime_error("error from arduino while reading sensor value.");
		}
	}
	else
	{
		throw new runtime_error("Timeout in Sense(). No response from arduino.");
	}

	return *value;
}

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

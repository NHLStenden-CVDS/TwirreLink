/*
 * TwirreLib.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include <unistd.h>
#include <iostream>

#include "TwirreLib.h"

using namespace twirre;

int main()
{
	TwirreLib twirre("/dev/ttyACM1");

	std::cout << (twirre.HaveSensor("sonar1") ? "yes" : "no") << std::endl;

	while(true)
	{
		auto& vals = twirre.GetSensor("sonar1")["distanceValues"];
		std::cout << vals.getSize() << ":";
		for(int i = 0; i < vals.getSize(); i++)
		{
			std::cout << " " << vals.as_uint16_t(i);
		}
		std::cout << std::endl;
	}
}

namespace twirre
{

TwirreLib::TwirreLib(const char * device)
{
	//Initialize serial port
	if (_serial.Initialize(device, 115200) < 0)
		throw std::runtime_error("Twirre initialization failed: failed to init serial port");

	//Wait for arduino to initialize the connection
	sleep(1);

	//Flush file descriptor (discard read/write data)
	_serial.flush();

	//Initialize sensors and actuators
	if(!_InitActuators())
		throw std::runtime_error("Twirre initialization failed: could not init actuators");
	if(!_InitSensors())
		throw std::runtime_error("Twirre initialization failed: could not init sensors");
}

TwirreLib::~TwirreLib()
{

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
	_serial.Write<ia>(m);
	sleep(1);
	std::string s;
	if(_serial.readString(s))
	{
		if(_ProcessInitString<Actuator>(s, _actuatorList))
		{
			return true;
		}
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
	_serial.Write<is>(m);
	sleep(1);
	std::string s;
	if(_serial.readString(s))
	{
		if(_ProcessInitString<Sensor>(s, _sensorList))
		{
			return true;
		}
	}
	return false;
}

template<typename T> bool TwirreLib::_ProcessInitString(const string & s, map<string, T*> &deviceList)
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

			for (size_t i = 0; i < deviceStrings.size(); ++i)
			{
				std::vector<std::string> deviceInformation;
				Helper::split(deviceStrings[i], '|', deviceInformation);

				// input: id, name, description, serial interface
				T* device = new T(i, deviceInformation[0], deviceInformation[1], _serial, deviceInformation[2]);

				cout << device->ToString() << endl;

				deviceList.insert(pair<string, T*>(deviceInformation[0], device));
			}
		}
		else return false;
	}
	return true;
}

bool TwirreLib::HaveSensor(const string & sensorName) const
{
	return (_sensorList.find(sensorName) != _sensorList.end());
}

bool TwirreLib::HaveActuator(const string & actuatorName) const
{
	return (_actuatorList.find(actuatorName) != _actuatorList.end());
}

Actuator& TwirreLib::GetActuator(const string & actuatorName)
{
	if(_actuatorList.find(actuatorName) == _actuatorList.end()){
		throw runtime_error("GetActuator: actuator id out of bounds");
	}
	return *_actuatorList.at(actuatorName);
}

Sensor& TwirreLib::GetSensor(const string & sensorName)
{
	if(_sensorList.find(sensorName) == _sensorList.end()){
		throw runtime_error("GetSensor: sensor id out of bounds");
	}
	return *_sensorList.at(sensorName);
}

bool TwirreLib::Ping()
{
	TwirreLib::_serial.Write('P');
	return _serial.Read<char>() == 'P';
}

bool TwirreLib::CheckOk(SerialRW & serialRW)
{
	char checkChar;
	serialRW.Read<char>(checkChar);

	if(checkChar == 'O')
	{
		return true;
	}
	else if(checkChar == 'E')
	{
		string error = "";
		serialRW.readString(error);
		cerr << error << endl;
	}
	else
	{
		//protocol error
		std::cerr << "CheckOK: Protocol Error, resetting serial" << std::endl;
		usleep(60000);
		serialRW.flush();
	}
	return false;
}
} /* namespace twirre */

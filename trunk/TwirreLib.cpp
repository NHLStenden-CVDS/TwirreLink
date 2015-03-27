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

	while(true)
	{
		auto vals = twirre.GetSensor("myAHRS+")[{"pitch", "roll", "yaw"}];

		if(vals.size() == 3)
		{
			std::cout << vals["pitch"]->as_float() << " " << vals["roll"]->as_float() << " " << vals["yaw"]->as_float() << " " << std::endl;
		}


		//usleep(50000);
	}
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
				T device(i, deviceInformation[0], deviceInformation[1], _soiw, deviceInformation[2]);

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


Actuator& TwirreLib::GetActuator(string actuatorName)
{
	if(_actuatorList.find(actuatorName) == _actuatorList.end()){
		throw runtime_error("GetActuator: actuator id out of bounds");
	}
	return _actuatorList.at(actuatorName);
}

Sensor& TwirreLib::GetSensor(string sensorName)
{
	if(_sensorList.find(sensorName) == _sensorList.end()){
		throw runtime_error("GetSensor: sensor id out of bounds");
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
		return false;
	}
	else
	{
		throw runtime_error("CheckOk: protocol error");
	}
}
} /* namespace twirre */

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
	twirre.init("/dev/ttyACM0");
	return 0;
}

namespace twirre
{

TwirreLib::TwirreLib()
{
}

bool TwirreLib::init(char * device)
{
	if (TwirreLib::_soiw.Initialize(device, 115200) == -1)
	{
		return false;
	}
	cout << "Serial port initialized." << endl;
	sleep(1);
	TwirreLib::_soiw.flush();

	_InitActuators();
	_InitSensors();
	cout << "Twirre Library ready to use!" << endl;
	return true;
}

void TwirreLib::_InitActuators()
{
	unsigned char pchar[2] =
	{ 'I', 'A' };
	TwirreLib::_soiw.writeBytes(pchar, 2);
	sleep(1);
	std::string s = TwirreLib::_soiw.readString();
	_actuatorList = _ProcessInitString<Actuator>(s);
	cout << "Actuators initialized. Number of actuators: "
			<< _actuatorList.size() << endl;
}

void TwirreLib::_InitSensors()
{
	unsigned char pchar[2] =
	{ 'I', 'S' };
	TwirreLib::_soiw.writeBytes(pchar, 2);
	sleep(1);
	std::string s = TwirreLib::_soiw.readString();
	_sensorList = _ProcessInitString<Sensor>(s);
	cout << "Sensors initialized. Number of sensors: " << _sensorList.size()
			<< endl;

}

template<typename T> std::vector<T> TwirreLib::_ProcessInitString(string & s)
{
	std::vector<T> deviceList;
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

				deviceList.push_back(device);
			}
		}
		else if (responseCode == 'E')
		{
			cout << "Error received from Arduino: " << payloadString << endl;
		}
		else
		{
			cout << "NOOOOOOOOOOOOOOOOOOO!" << endl;
			// something broke quite hard
		}
	}
	return deviceList;
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

		Value value(nameAndType[1]);

		values.insert(pair<string, Value>(nameAndType[0], value));
	}

	return values;
}

std::vector<Actuator> TwirreLib::GetActuatorList()
{
	return TwirreLib::_actuatorList;
}

std::vector<Sensor> TwirreLib::GetSensorList()
{
	return TwirreLib::_sensorList;
}

Actuator TwirreLib::GetActuator(int n)
{
	if (n < _actuatorList.size())
	{
		return _actuatorList[n];
	}
	else
	{
		throw new runtime_error("actuator id out of bounds");
	}
}

Sensor TwirreLib::GetSensor(int n)
{
	if (n < _sensorList.size())
	{
		return _sensorList[n];
	}
	else
	{
		throw new runtime_error("sensor id out of bounds");
	}
}

bool TwirreLib::Ping()
{
	TwirreLib::_soiw.Write('P');
	return TwirreLib::_soiw.Read<char>() == 'P';
}

TwirreLib::~TwirreLib()
{
}

} /* namespace twirre */

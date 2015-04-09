/*
 * TwirreLib.cpp
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include <unistd.h>
#include <iostream>
#include <chrono>

#include "TwirreSerial.h"
#include "Support/Helper.h"

using namespace twirre;
using namespace std;

int main()
{
	TwirreSerial twirre("/dev/ttyACM0");

	bool haveNaza = twirre.HaveActuator("naza");

	while (true)
	{
		auto imuvals = twirre.GetSensor("myAHRS+")[{"pitch", "yaw", "roll"}];
		//print the imu values
		for (auto & val : imuvals)
		{
			std::cout << val.first << ":\t" << val.second->as_int16_t() << "\t";
		}
		auto& distvals = twirre.GetSensor("sonar1")["distanceValues"];
		std::cout << "alt:\t" << distvals.as_int16_t();

		std::cout << std::endl;

		if (haveNaza)
		{

			auto& naza = twirre.GetActuator("naza");

			 //set the naza actuator values
			 naza["pitch"] = (imuvals["pitch"]->as_float() / 180.0f);
			 naza["yaw"] = (imuvals["yaw"]->as_float() / 180.0f);
			 naza["roll"] = (imuvals["roll"]->as_float() / 180.0f);
			 naza["gaz"] = (distvals.as_float(0) / 100.0f) - 1.0f;
			 naza["timeout"] = 10000;

			 naza.Actuate(); //send updated values to the naza actuator
		}
	}
}

namespace twirre
{
	TwirreSerial::TwirreSerial(const char * device, const int baudrate)
	{
		//Initialize serial port
		if (_serial.Initialize(device, baudrate) < 0) throw std::runtime_error("Twirre initialization failed: failed to init serial port");

		//Wait for arduino to initialize the connection
		sleep(1);

		//Flush file descriptor (discard read/write data)
		_serial.flush();

		//Initialize sensors and actuators
		if (!_InitActuators()) throw std::runtime_error("Twirre initialization failed: could not init actuators");
		if (!_InitSensors()) throw std::runtime_error("Twirre initialization failed: could not init sensors");
	}

	TwirreSerial::~TwirreSerial()
	{

	}

	bool TwirreSerial::_InitActuators()
	{
		unsigned char iamsg[] = {'I', 'A'};
		_serial.writeBytes(iamsg, 2);
		sleep(1);
		std::string s;
		if (_serial.readString(s))
		{
			if (_ProcessInitString<Actuator>(s, _actuatorList))
			{
				return true;
			}
		}
		return false;
	}

	bool TwirreSerial::_InitSensors()
	{
		struct is
		{
			char i = 'I';
			char s = 'S';
		};
		is m;
		_serial.Write<is>(m);
		sleep(1);
		std::string s;
		if (_serial.readString(s))
		{
			if (_ProcessInitString<Sensor>(s, _sensorList))
			{
				return true;
			}
		}
		return false;
	}

	template<typename T> bool TwirreSerial::_ProcessInitString(const string & s, map<string, T*> &deviceList)
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
			else
				return false;
		}
		return true;
	}

	bool TwirreSerial::HaveSensor(const string & sensorName) const
	{
		return (_sensorList.find(sensorName) != _sensorList.end());
	}

	bool TwirreSerial::HaveActuator(const string & actuatorName) const
	{
		return (_actuatorList.find(actuatorName) != _actuatorList.end());
	}

	Actuator& TwirreSerial::GetActuator(const string & actuatorName)
	{
		if (_actuatorList.find(actuatorName) == _actuatorList.end())
		{
			throw runtime_error("GetActuator: no actuator with that name");
		}
		return *_actuatorList.at(actuatorName);
	}

	Sensor& TwirreSerial::GetSensor(const string & sensorName)
	{
		if (_sensorList.find(sensorName) == _sensorList.end())
		{
			throw runtime_error("GetSensor: no sensor with that name");
		}
		return *_sensorList.at(sensorName);
	}

	bool TwirreSerial::Ping()
	{
		TwirreSerial::_serial.Write('P');
		return _serial.Read<char>() == 'P';
	}

	bool TwirreSerial::CheckOk(SerialRW & serialRW)
	{
		char checkChar;
		serialRW.Read<char>(checkChar);

		if (checkChar == 'O')
		{
			return true;
		}
		else if (checkChar == 'E')
		{
			string error = "";
			serialRW.readString(error);
			cerr << error << endl;
		}
		else
		{
			//protocol error
			std::cerr << "CheckOK: Protocol Error, resetting serial" << std::endl;
			usleep(250000);
			serialRW.flush();
		}
		return false;
	}
} /* namespace twirre */

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

#include <unistd.h>
#include <iostream>
#include <chrono>

#include "../TwirreSerial/SerialActuator.h"
#include "../TwirreSerial/SerialSensor.h"
#include "../Support/Helper.h"

#include "../TwirreSerial/TwirreSerial.h"

using namespace twirre;
using namespace std;



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

		//this will setup logging
		doNotifyChange();
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
			if (_ProcessInitString<Actuator, SerialActuator>(s, _actuatorList))
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
			if (_ProcessInitString<Sensor, SerialSensor>(s, _sensorList))
			{
				return true;
			}
		}
		return false;
	}

	template<typename T, typename V> bool TwirreSerial::_ProcessInitString(const string & s, map<string, T*> &deviceList)
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
					T* device = new V(i, deviceInformation[0], deviceInformation[1], _serial, deviceInformation[2], _serialMutex);

					cout << device->ToString() << endl;

					deviceList.insert(pair<string, T*>(deviceInformation[0], device));
				}
			}
			else
				return false;
		}
		return true;
	}

	bool TwirreSerial::Ping()
	{
		std::lock_guard<std::mutex> lock(_serialMutex);

		_serial.Write('P');
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

	const std::map<std::string, Actuator*> & TwirreSerial::getActuators()
	{
		return _actuatorList;
	}

	const std::map<std::string, Sensor*> & TwirreSerial::getSensors()
	{
		return _sensorList;
	}
} /* namespace twirre */

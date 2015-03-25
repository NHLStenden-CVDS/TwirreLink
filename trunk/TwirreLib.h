/*
 * TwirreLib.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef TWIRRELIB_H_
#define TWIRRELIB_H_
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "Serial/SerialRW.h"
#include "Support/Helper.h"
#include "Core/Value.h"

using namespace std;

namespace twirre
{
class TwirreLib
{
public:
	TwirreLib();
	virtual ~TwirreLib();
	bool Init(const char*);

	bool Ping();
	Value* Sense(string sensorName, string valueName);

	Sensor& GetSensor(string sensorName);
	Actuator& GetActuator(string actuatorName);

	static bool CheckOk(SerialRW & serialRW);

private:
	#pragma pack(push, 1) //set 1-byte element alignment (effectively disables automatic struct padding)
	struct MessageHeader{
			char opcode;
			char targetID;
			uint16_t payloadSize;
	};
	#pragma pack(pop)

	map<string, Actuator> _actuatorList;
	map<string, Sensor> _sensorList;
	SerialRW _soiw;

	template<typename T> bool _ProcessInitString(string & s, map<string, T> &deviceList);
	bool _InitActuators();
	bool _InitSensors();
	map<string, Value*> _ProcessValuesString(string & s);

};

} /* namespace twirre */

#endif /* TWIRRELIB_H_ */

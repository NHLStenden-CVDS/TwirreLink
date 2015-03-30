/*
 * TwirreLib.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#ifndef TWIRRELIB_H_
#define TWIRRELIB_H_



#include <cstdint>
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
	friend class Sensor;
	friend class Actuator;

public:
	TwirreLib(const char* path);
	~TwirreLib();

	bool Ping();

	bool HaveSensor(const string & sensorName) const;
	bool HaveActuator(const string & actuatorName) const;

	Sensor& GetSensor(const string & sensorName);
	Actuator& GetActuator(const string & actuatorName);

private:
	static bool CheckOk(SerialRW & serialRW);

	map<string, Actuator*> _actuatorList;
	map<string, Sensor*> _sensorList;
	SerialRW _serial;

	template<typename T> bool _ProcessInitString(const string & s, map<string, T*> &deviceList);
	bool _InitActuators();
	bool _InitSensors();
	map<string, Value*> _ProcessValuesString(const string & s);

};

} /* namespace twirre */

#endif /* TWIRRELIB_H_ */

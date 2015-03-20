/*
 * TwirreLib.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef TWIRRELIB_H_
#define TWIRRELIB_H_
#include <unistd.h>
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
	bool init(char*);

	bool Ping();

	Sensor GetSensor(int);
	Actuator GetActuator(int);

	vector<Actuator> GetActuatorList(void);
	vector<Sensor> GetSensorList(void);

private:
	enum DeviceType{SENSOR, ACTUATOR};

	vector<Actuator> _actuatorList;
	vector<Sensor> _sensorList;
	SerialRW _soiw;

	template<typename T> std::vector<T> _ProcessInitString(string & s);
	void _InitActuators();
	void _InitSensors();
	map<string, Value> _ProcessValuesString(string & s);

};

} /* namespace twirre */

#endif /* TWIRRELIB_H_ */

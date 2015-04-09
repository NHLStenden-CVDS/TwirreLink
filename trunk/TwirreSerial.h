/*
 * TwirreLib.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#ifndef TWIRRESERIAL_H_
#define TWIRRESERIAL_H_

#include <vector>
#include <string>

#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "Core/Value.h"

#include "Serial/SerialRW.h"

namespace twirre
{
class TwirreSerial
{
	friend class Sensor;
	friend class Actuator;

public:
	explicit TwirreSerial(const char* path, const int baudrate = 115200);
	~TwirreSerial();

	bool Ping();

	bool HaveSensor(const std::string & sensorName) const;
	bool HaveActuator(const std::string & actuatorName) const;

	Sensor& GetSensor(const std::string & sensorName);
	Actuator& GetActuator(const std::string & actuatorName);

private:
	static bool CheckOk(SerialRW & serialRW);

	std::map<std::string, Actuator*> _actuatorList;
	std::map<std::string, Sensor*> _sensorList;
	SerialRW _serial;

	template<typename T> bool _ProcessInitString(const std::string & s, std::map<std::string, T*> &deviceList);
	bool _InitActuators();
	bool _InitSensors();
	std::map<std::string, Value*> _ProcessValuesString(const std::string & s);

};

} /* namespace twirre */

#endif /* TWIRRESERIAL_H_ */

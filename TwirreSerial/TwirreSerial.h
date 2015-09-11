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
#include <mutex>

#include "../Core/Actuator.h"
#include "../Core/Sensor.h"
#include "../Core/Value.h"

#include "../Serial/SerialRW.h"

#include "../DeviceProvider.h"

namespace twirre
{
class TwirreSerial : public DeviceProvider
{
	friend class SerialSensor;
	friend class SerialActuator;

public:
	explicit TwirreSerial(const char* path, const int baudrate = 115200);
	~TwirreSerial();

	bool Ping();

private:
	static bool CheckOk(SerialRW & serialRW);

	std::map<std::string, Actuator*> _actuatorList;
	std::map<std::string, Sensor*> _sensorList;
	SerialRW _serial;
	std::mutex _serialMutex; //mutex for all post-init serial actions

	template<typename T, typename V> bool _ProcessInitString(const std::string & s, std::map<std::string, T*> &deviceList);
	bool _InitActuators();
	bool _InitSensors();
	std::map<std::string, Value*> _ProcessValuesString(const std::string & s);

	virtual const std::map<std::string, Actuator*> & getActuators() override;
	virtual const std::map<std::string, Sensor*> & getSensors() override;

};

} /* namespace twirre */

#endif /* TWIRRESERIAL_H_ */

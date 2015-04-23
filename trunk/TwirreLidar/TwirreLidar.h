/*
 * TwirreLidar.h
 *
 *  Created on: Apr 23, 2015
 *      Author: Jasper
 */

#ifndef TWIRRELIDAR_H_
#define TWIRRELIDAR_H_

#include "DeviceProvider.h"
#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "Core/Value.h"

namespace twirre
{
class TwirreLidar : public DeviceProvider
{

public:
	explicit TwirreLidar(const char* path, const int baudrate = 115200);
	~TwirreLidar();

private:
	std::map<std::string, Actuator*> _actuatorList;
	std::map<std::string, Sensor*> _sensorList;

	bool _InitActuators();
	bool _InitSensors();

	virtual const std::map<std::string, Actuator*> & getActuators() override;
	virtual const std::map<std::string, Sensor*> & getSensors() override;
};

} /* namespace twirre */

#endif /* TWIRRELIDAR_H_ */

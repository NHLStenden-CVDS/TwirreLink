/*
 * RPLidar.h
 *
 *  Created on: Apr 23, 2015
 *      Author: jasper
 */

#ifndef RPLIDAR_H_
#define RPLIDAR_H_

#include <string>
#include "Sensor.h"
#include <rplidar.h>

#define RPLIDAR_MEASUREMENT_COUNT 2048
#define RPLIDAR_DEGTORAD_FACTOR 0.0174532925f

namespace twirre
{

class RPLidarSensor: public Sensor
{
public:
	RPLidarSensor(const char* path, std::string name);
	virtual ~RPLidarSensor();
	virtual std::map<std::string, Value*> Sense(const std::vector<std::string> &names) override;
private:
	ArrayValue<float> * _distanceVals;
	ArrayValue<float> * _angleVals;

	rp::standalone::rplidar::RPlidarDriver * drv;
	unsigned long int _measurementCount;
	rplidar_response_measurement_node_t _measurementBuffer[RPLIDAR_MEASUREMENT_COUNT];
};

}
#endif /* RPLIDAR_H_ */

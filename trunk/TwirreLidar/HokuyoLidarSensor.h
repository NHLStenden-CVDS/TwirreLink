/*
 * RPLidar.h
 *
 *  Created on: Apr 23, 2015
 *      Author: jasper
 */

#ifndef HOKUYOLIDARSENSOR_H_
#define HOKUYOLIDARSENSOR_H_

#include <string>
#include "Sensor.h"

//URG library
//http://urgnetwork.sourceforge.net/html/
#include <urg_sensor.h>
#include <urg_utils.h>

namespace twirre
{

class HokuyoLidarSensor: public Sensor
{
public:
	HokuyoLidarSensor(const char* path, std::string name);
	virtual ~HokuyoLidarSensor();
	virtual std::map<std::string, Value*> Sense(const std::vector<std::string> &names) override;
private:
	//urg lidar object
	urg_t urg;
	int urgMaxDataCount;

	//parameters
	URGLidarSnapMode autoSnapMode;
	int mmPerPixel;


	ArrayValue<float> * _angleVals;

	unsigned long int _measurementCount;
};

}
#endif /* RPLIDAR_H_ */

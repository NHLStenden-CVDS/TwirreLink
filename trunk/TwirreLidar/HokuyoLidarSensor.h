/*
 * RPLidar.h
 *
 *  Created on: Apr 23, 2015
 *      Author: jasper
 */

#ifndef HOKUYOLIDARSENSOR_H_
#define HOKUYOLIDARSENSOR_H_

//C++11 threading
#include <thread>
#include <mutex>
#include <atomic>

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

	int measurementCount;
	int measurementCount_buf;
	long * urgMeasurementData;
	long * urgMeasurementData_buf;

	ArrayValue<int64_t> * _distanceVals;


	bool _freshImage;
	std::atomic<bool> _threadRun;

	std::thread * _thread = nullptr;
	std::mutex * _mutex = nullptr;


	//thread functions
	int updateReading();
	void thread_main();
};

}
#endif /* RPLIDAR_H_ */

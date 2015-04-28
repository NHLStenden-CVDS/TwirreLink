/*
 * RPLidar.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: jasper
 */

#include <HokuyoLidarSensor.h>
#include <Value.h>
#include <map>
#include <cstring>
#include <iostream>

//URG library
//http://urgnetwork.sourceforge.net/html/
#include <urg_sensor.h>
#include <urg_utils.h>

using namespace std;

namespace twirre
{

HokuyoLidarSensor::HokuyoLidarSensor(const char* path, string name) :
		Sensor(name, "is a Hokuyo Lidar")
{

	uint32_t opt_com_baudrate = 115200;
	int ret;

	// create the driver instance
	ret = urg_open(&urg, URG_SERIAL, path, opt_com_baudrate);

	if (ret < 0)
	{
		fprintf(stderr, "error opening URG device\n");
		throw runtime_error("[URGLidar] error opening URG device");
	}

	//allocate data field for measurements
	urgMaxDataCount = urg_max_data_size(&urg);
	urgMeasurementData = (long *) malloc(sizeof(long) * urgMaxDataCount);
	urgMeasurementData_buf = (long *) malloc(sizeof(long) * urgMaxDataCount);
	measurementCount = 0;
	measurementCount_buf = 0;

	//get min and max step number
	int minstep;
	int maxstep;
	urg_step_min_max(&urg, &minstep, &maxstep);

	ret = urg_set_scanning_parameter(&urg, minstep, maxstep, 0);
	if (ret < 0)
	{
		std::cerr << "Failed to set URG scanning parameters: " << ret << std::endl;
	}


	_distanceVals = new ArrayValue<int64_t>("distances");

	_valueList[_distanceVals->getName()] = _distanceVals;

	//start the thread
	_threadRun = true;
	_freshImage = false;
	_mutex = new std::mutex();
	_thread = new std::thread(&HokuyoLidarSensor::thread_main, this);
}

HokuyoLidarSensor::~HokuyoLidarSensor()
{
	_threadRun = false;
	_thread->join();

	delete _mutex;
	delete _thread;

	free(urgMeasurementData_buf);
	free(urgMeasurementData);

	urg_close(&urg);
}

map<string, Value*> HokuyoLidarSensor::Sense(const std::vector<std::string> &names)
{
	map<string, Value*> valuesMap;

	bool wantNew = false;
	for (const auto & name : names)
	{
		if (!name.compare(_distanceVals->getName()))
		{
			wantNew = true;
			valuesMap[_distanceVals->getName()] = _distanceVals;
		}
		else
			valuesMap[name] = ErrorValue::getInstance();
	}

	if (wantNew)
	{
		std::lock_guard<mutex> lock(*_mutex);

		if(_freshImage)
		{
			_distanceVals->setSize(measurementCount);
			int64_t * distBuf = _distanceVals->getNativeBuffer();
			memcpy(distBuf, urgMeasurementData, sizeof(int64_t) * measurementCount);
		}
	}

	return valuesMap;
}

int HokuyoLidarSensor::updateReading()
{
	measurementCount_buf = urg_get_distance(&urg, urgMeasurementData_buf, NULL);

	if (measurementCount_buf < 0)
	{
		urg_stop_measurement(&urg);
		int ret = urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, 0);
		if (ret < 0)
		{
			fprintf(stderr, "failed to restart measurements with URG device\n");
		}
		return -1;
	}

	return 1;
}

void HokuyoLidarSensor::thread_main()
	{
		std::cout << "thread started" << std::endl;

		//init
		// start scan...
		int ret = urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, 0);

		std::cout << "start measurement done" << std::endl;

		if (ret < 0)
		{
			fprintf(stderr, "failed to start measurements with URG device\n");
			_threadRun = false;
		}

		//mainloop
		while (_threadRun)
		{
			updateReading();

			if (measurementCount_buf > 0)
			{
				std::lock_guard<std::mutex> lock(*_mutex);
				measurementCount = measurementCount_buf;
				std::memcpy(urgMeasurementData, urgMeasurementData_buf, sizeof(long) * measurementCount_buf);
				_freshImage = true;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		//exit
		urg_stop_measurement(&urg);
	}

}


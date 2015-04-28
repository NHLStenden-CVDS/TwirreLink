/*
 * RPLidar.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: jasper
 */

#include <HokuyoLidarSensor.h>
#include <Value.h>
#include <map>

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

	// make connection...
	if (IS_FAIL(drv->connect(path, opt_com_baudrate)))
	{
		throw std::runtime_error("Failed to bind to serial port");
	}

	// check health...
	//if (!checkRPLIDARHealth(drv))
	//{
	//	throw Error("RPLidar health check failed");
	//}

	// start scan...
	drv->startScan();

	_distanceVals = new ArrayValue<float>("distances");
	_angleVals = new ArrayValue<float>("angles");

	_valueList[_distanceVals->getName()] = _distanceVals;
	_valueList[_angleVals->getName()] = _angleVals;
}

RPLidarSensor::~RPLidarSensor()
{
	RPlidarDriver::DisposeDriver(drv);
}

map<string, Value*> RPLidarSensor::Sense(const std::vector<std::string> &names)
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
		else if (!name.compare(_angleVals->getName()))
		{
			valuesMap[_angleVals->getName()] = _angleVals;
			wantNew = true;
		}
		else
			valuesMap[name] = ErrorValue::getInstance();
	}

	if (wantNew)
	{
		_measurementCount = RPLIDAR_MEASUREMENT_COUNT;
		uint32_t op_result = drv->grabScanData(_measurementBuffer,
				_measurementCount);

		if (IS_OK(op_result))
		{
			drv->ascendScanData(_measurementBuffer, _measurementCount);
		}

		_distanceVals->setSize(_measurementCount);
		_angleVals->setSize(_measurementCount);

		float* distBuf = _distanceVals->getNativeBuffer();
		float* angleBuf = _angleVals->getNativeBuffer();

		for(unsigned int i = 0; i < _measurementCount; i++)
		{
			distBuf[i] = _measurementBuffer[i].distance_q2 / 4.0f;
			angleBuf[i] = (_measurementBuffer[i].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f;
		}
	}

	return valuesMap;
}

}


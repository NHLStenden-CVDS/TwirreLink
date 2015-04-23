/*
 * TwirreLidar.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: Jasper
 */

#include "TwirreLidar/RPLidarSensor.h"
#include "TwirreLidar/TwirreLidar.h"

using namespace twirre;
using namespace std;

namespace twirre
{
TwirreLidar::TwirreLidar()
{

}

TwirreLidar::~TwirreLidar()
{

}

void TwirreLidar::ConnectRPLidar(const char* path, std::string name)
{
	_sensorList[name] = new RPLidarSensor(path, name);
}

const std::map<std::string, Actuator*> & TwirreLidar::getActuators()
{
	return _actuatorList;
}

const std::map<std::string, Sensor*> & TwirreLidar::getSensors()
{
	return _sensorList;
}
} /* namespace twirre */

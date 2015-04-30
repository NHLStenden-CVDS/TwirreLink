/*
 * TwirreLidar.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: Jasper
 */

#include "TwirreLidar/RPLidarSensor.h"
#include "TwirreLidar/HokuyoLidarSensor.h"
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
	for(const auto & pair : _sensorList)
	{
		delete pair.second;
	}
}

void TwirreLidar::ConnectRPLidar(const char* path, std::string name)
{
	if(_sensorList.find(name) != _sensorList.end()) delete _sensorList[name];
	_sensorList[name] = new RPLidarSensor(path, name);
	doNotifyChange();
}

void TwirreLidar::ConnectHokuyoLidar(const char* path, std::string name)
{
	if(_sensorList.find(name) != _sensorList.end()) delete _sensorList[name];
	_sensorList[name] = new HokuyoLidarSensor(path, name);
	doNotifyChange();
}

void TwirreLidar::DisconnectLidar(std::string name)
{
	if(_sensorList.find(name) != _sensorList.end()) delete _sensorList[name];
	_sensorList.erase(name);
	doNotifyChange();
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

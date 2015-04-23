/*
 * TwirreLidar.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: Jasper
 */

#include "TwirreLidar.h"

using namespace twirre;
using namespace std;



namespace twirre
{
	TwirreLidar::TwirreLidar(const char * device, const int baudrate)
	{

	}

	TwirreLidar::~TwirreLidar()
	{

	}

	bool TwirreLidar::_InitActuators()
	{

	}

	bool TwirreLidar::_InitSensors()
	{

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

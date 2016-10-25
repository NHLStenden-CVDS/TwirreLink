/*
 * TwirreLogger.h
 *
 *  Created on: Oct 7, 2016
 *      Author: mrd
 */

#ifndef LOGGER_TWIRRELOGGER_H_
#define LOGGER_TWIRRELOGGER_H_

#include "../Core/Actuator.h"
#include "../Core/Sensor.h"

#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <chrono>

namespace twirre
{
	class TwirreLogger
	{
	public:
		TwirreLogger(const std::string & logpath);
		virtual ~TwirreLogger();

		void logActuators(std::map<std::string, Actuator*>& actuators);
		void logSensors(std::map<std::string, Sensor*>& sensors);
		void logSensorEvent(Sensor * sensor, std::map<std::string, Value *> sensorValues);
		void logActuatorEvent(Actuator * actuator, std::map<std::string, Parameter *> actuatorParameters);

		void onDevicelistChanged(void);

		uint64_t getTimestamp(void);



	private:
		std::shared_ptr<std::ofstream> _logfile;
		std::chrono::time_point<std::chrono::steady_clock> _tp_start;
	};
}
#endif /* LOGGER_TWIRRELOGGER_H_ */

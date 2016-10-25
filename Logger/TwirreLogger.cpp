/*
 * TwirreLogger.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: mrd
 */

#include "TwirreLogger.h"

#include <iostream>

using namespace std;

namespace twirre
{

	TwirreLogger::TwirreLogger(const string & logpath)
	{
		_tp_start = std::chrono::steady_clock::now();
		_logfile = make_shared<ofstream>();
		_logfile->open(logpath);
		*_logfile << "# TwirreLogger initializing" << endl;
		*_logfile << getTimestamp() << " init" << endl;
	}

	TwirreLogger::~TwirreLogger()
	{
		*_logfile << getTimestamp() << " stop" << endl;
		*_logfile << "# TwirreLogger stopped" << endl;
		_logfile->close();
	}

	void TwirreLogger::logActuators(std::map<std::string, Actuator*>& actuators)
	{
		*_logfile << "# Full actuator list" << endl;
		*_logfile << getTimestamp() << " actuators {" << endl;

		for(auto & actuator : actuators)
		{
			*_logfile << "\t" << actuator.second->getName() << " {" << endl;

			auto parameters = actuator.second->GetParameters(actuator.second->getAvailableParameters());
			for(auto parameter : parameters)
			{
				string name = parameter.second->getName();
				NativeType type = parameter.second->getNativeType();
				bool isArray = parameter.second->isArray();
				*_logfile << "\t\t" << name << ":";
				if(isArray) {
					*_logfile << "array_";
				}
				*_logfile << enumtostr(type) << endl;
			}

			*_logfile << "\t}" << endl;
		}

		*_logfile << "}" << endl;
		*_logfile << "# end of full actuator list" << endl;
	}

	void TwirreLogger::logSensors(std::map<std::string, Sensor*>& sensors)
	{
		*_logfile << "# Full sensor list" << endl;
		*_logfile << getTimestamp() << " sensors {" << endl;

		for(auto & sensor : sensors)
		{
			*_logfile << "\t" << sensor.second->getName() << " {" << endl;

			auto values = sensor.second->peekValues(sensor.second->getAvailableValues());
			for(auto value : values)
			{
				string name = value.second->getName();
				NativeType type = value.second->getNativeType();
				bool isArray = value.second->isArray();
				*_logfile << "\t\t" << name << ":";
				if(isArray) {
					*_logfile << "array_";
				}
				*_logfile << enumtostr(type) << endl;
			}

			*_logfile << "\t}" << endl;
		}

		*_logfile << "}" << endl;
		*_logfile << "# end of full sensor list" << endl;
	}

	void TwirreLogger::logSensorEvent(Sensor * sensor, std::map<std::string, Value *> sensorValues)
	{
		*_logfile << getTimestamp() << " sense " << sensor->getName() << " {" << endl;

		for(auto & value : sensorValues)
		{
			*_logfile << "\t" << value.second->getName() << ":";

			if(value.second->isArray())
			{
				*_logfile << "array; logging not implemented yet";
			}
			else
			{
				*_logfile << value.second->as_string();
			}

			*_logfile << endl;
		}

		*_logfile << "}" << endl;
	}

	void TwirreLogger::logActuatorEvent(Actuator * actuator, std::map<std::string, Parameter *> actuatorParameters)
	{
		*_logfile << getTimestamp() << " actuate " << actuator->getName() << " {" << endl;

		for(auto & param : actuatorParameters)
		{
			*_logfile << "\t" << param.second->getName() << ":";

			if(param.second->isArray())
			{
				*_logfile << "array; logging not implemented yet";
			}
			else
			{
				*_logfile << param.second->as_string();
			}

			*_logfile << endl;
		}

		*_logfile << "}" << endl;
	}

	void TwirreLogger::onDevicelistChanged()
	{
		*_logfile << "# TwirreLink device list changed" << endl;
		*_logfile << getTimestamp() << "devicelist_changed" << endl;
	}

	uint64_t TwirreLogger::getTimestamp(void)
	{
		auto now = std::chrono::steady_clock::now();
		auto timestamp = now - _tp_start;
		return std::chrono::duration_cast<std::chrono::microseconds>(timestamp).count();
	}
}

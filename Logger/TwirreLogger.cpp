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

	TwirreLogger::TwirreLogger(const string & logpath, const string & binpath)
	{
		_tp_start = std::chrono::steady_clock::now();
		_logfile = make_shared<ofstream>();
		_logfile->open(logpath);
		if(!_logfile->is_open())
		{
			throw std::runtime_error("TwirreLogger: failed to create tlog file");
		}

		*_logfile << "# TwirreLogger initializing" << endl;
		*_logfile << getTimestamp() << " init" << endl;

		//init binary file
		_binfile = make_shared<ofstream>();
		_binfile->open(binpath, std::ofstream::binary);
		if(!_binfile->is_open())
		{
			throw std::runtime_error("TwirreLogger: failed to create tbin file");
		}

		*_logfile << getTimestamp() << " create binfile " << binpath << std::endl;
	}

	TwirreLogger::~TwirreLogger()
	{
		//close logfile
		{
			std::lock_guard<std::mutex> logMutexLock (_logfileMutex);

			*_logfile << getTimestamp() << " stop" << endl;
			*_logfile << "# TwirreLogger stopped" << endl;
			_logfile->close();
		}
		//close binfile
		{
			std::lock_guard<std::mutex> binMutexLock (_binfileMutex);

			_binfile->close();
		}
	}

	void TwirreLogger::logActuators(std::map<std::string, Actuator*>& actuators)
	{
		std::lock_guard<std::mutex> logMutexLock (_logfileMutex);

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
		std::lock_guard<std::mutex> logMutexLock (_logfileMutex);

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
		std::unique_lock<std::mutex> logMutexLock(_logfileMutex);
		*_logfile << getTimestamp() << " sense " << sensor->getName() << " {" << endl;

		for(auto & value : sensorValues)
		{
			*_logfile << "\t" << value.second->getName() << ":";

			if(value.second->isArray())
			{
				*_logfile << "array (" << _binaryDataOffset << "," << value.second->getSize() << ")";

				//write to binfile. Temporarily release logfileMutex.
				logMutexLock.unlock();
				logBinArrayValue(value.second);
				logMutexLock.lock();

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
		std::unique_lock<std::mutex> logMutexLock(_logfileMutex);
		*_logfile << getTimestamp() << " actuate " << actuator->getName() << " {" << endl;

		for(auto & param : actuatorParameters)
		{
			*_logfile << "\t" << param.second->getName() << ":";

			if(param.second->isArray())
			{
				*_logfile << "array (" << _binaryDataOffset << "," << param.second->getSize() << ")";

				logMutexLock.unlock();
				logBinArrayValue(dynamic_cast<Value *>(param.second));
				logMutexLock.lock();
			}
			else
			{
				*_logfile << param.second->as_string();
			}

			*_logfile << endl;
		}

		*_logfile << "}" << endl;
	}

	void TwirreLogger::logBinArrayValue(Value * val)
	{
		std::lock_guard<std::mutex> binMutexLock (_binfileMutex);

		auto dataptr = val->getBuffer();
		size_t nbytes = val->getSize() * val->getElementSize();
		_binfile->write(reinterpret_cast<char *>(dataptr), nbytes);
		_binaryDataOffset += nbytes;
	}

	void TwirreLogger::onDevicelistChanged()
	{
		std::lock_guard<std::mutex> logMutexLock (_logfileMutex);

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

/*
 * Twirre: architecture for autonomous UAVs using interchangeable commodity components
 *
 * Copyright© 2017 Centre of expertise in Computer Vision & Data Science, NHL Stenden University of applied sciences
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "TwirreLogger.h"

#include <iostream>
#include <sstream>
#include <cstring>

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
			*_logfile << getTimestamp() << "! error create binfile " << binpath << " : failed to create file" << endl;
			_logfile->close();
			throw std::runtime_error("TwirreLogger: failed to create tbin file");
		}

		*_logfile << getTimestamp() << " create binfile " << binpath << std::endl;

		//start async writer threads
		_runLogfileThread = true;
		_logfileThread = new thread(&TwirreLogger::logfileThreadMain, this);
		_runBinfileThread = true;
		_binfileThread = new thread(&TwirreLogger::binfileThreadMain, this);
	}

	TwirreLogger::~TwirreLogger()
	{
		//stop async writer threads
		{
			lock_guard<mutex> logfileLock(_logfileMutex);
			_runLogfileThread = false;
		}
		_logfileCV.notify_all();
		_logfileThread->join();
		delete _logfileThread;

		{
			lock_guard<mutex> binfileLock(_binfileMutex);
			_runBinfileThread = false;
		}
		_binfileCV.notify_all();
		_binfileThread->join();
		delete _binfileThread;

		//close logfile
		{
			*_logfile << getTimestamp() << " stop" << endl;
			*_logfile << "# TwirreLogger stopped" << endl;
			_logfile->close();
		}
		//close binfile
		{
			_binfile->close();
		}
	}

	void TwirreLogger::logActuators(std::map<std::string, Actuator*>& actuators)
	{
		stringstream logStream;

		logStream << "# Full actuator list" << endl;
		logStream << getTimestamp() << " actuators {" << endl;

		for(auto & actuator : actuators)
		{
			logStream << "\t" << actuator.second->getName() << " {" << endl;

			auto parameters = actuator.second->GetParameters(actuator.second->getAvailableParameters());
			for(auto parameter : parameters)
			{
				string name = parameter.second->getName();
				NativeType type = parameter.second->getNativeType();
				bool isArray = parameter.second->isArray();
				logStream << "\t\t" << name << ":";
				if(isArray) {
					logStream << "array_";
				}
				logStream << enumtostr(type) << endl;
			}

			logStream << "\t}" << endl;
		}

		logStream << "}" << endl;
		logStream << "# end of full actuator list" << endl;

		logString(logStream.str());
	}

	void TwirreLogger::logSensors(std::map<std::string, Sensor*>& sensors)
	{
		stringstream logStream;

		logStream << "# Full sensor list" << endl;
		logStream << getTimestamp() << " sensors {" << endl;

		for(auto & sensor : sensors)
		{
			logStream<< "\t" << sensor.second->getName() << " {" << endl;

			auto values = sensor.second->peekValues(sensor.second->getAvailableValues());
			for(auto value : values)
			{
				string name = value.second->getName();
				NativeType type = value.second->getNativeType();
				bool isArray = value.second->isArray();
				logStream << "\t\t" << name << ":";
				if(isArray) {
					logStream << "array_";
				}
				logStream << enumtostr(type) << endl;
			}

			logStream << "\t}" << endl;
		}

		logStream << "}" << endl;
		logStream << "# end of full sensor list" << endl;

		logString(logStream.str());
	}

	template<class T>
	string TwirreLogger::logDeviceValues(const std::map<std::string, T *> & values)
	{
		stringstream logStream;

		int errorCount = 0;
		for(auto & value : values)
		{
			if(value.second->isValid()) //if sense/actuate was called with non-existent value names, for those names a special errorValue is returned. Only log valid values for now.
			{
				logStream << "\t" << value.second->getName() << ":";

				if(value.second->isArray())
				{
					auto arraySize = value.second->getSize();
					if(arraySize <= _maxArraySize)
					{
						logStream << "array (" << _binaryDataOffset << "," << arraySize << ")";

						logBinArrayValue(dynamic_cast<Value *>(value.second));
					}
					else
					{
						logStream << "array_big (" << arraySize << ")";
					}
				}
				else
				{
					logStream << value.second->as_string();
				}

				logStream << endl;
			}
			else //count invalid values
			{
				errorCount++;
			}
		}

		//if errorValues are present, write an error to the logfile containing the count of errored values.
		if(errorCount > 0)
		{
			logStream << "!\t<error>:" << errorCount << (errorCount > 1 ? " errorvalues" : " errorvalue") << " present" << endl;
		}

		return logStream.str();
	}

	void TwirreLogger::logSensorEvent(Sensor * sensor, std::map<std::string, Value *> sensorValues)
	{
		stringstream logStream;

		logStream << getTimestamp() << " sense " << sensor->getName() << " {" << endl;

		logStream << logDeviceValues(sensorValues);

		logStream << "}" << endl;

		logString(logStream.str());
	}

	void TwirreLogger::manualSensorEvent(std::string sensorName, const std::vector<std::pair<std::string, std::string>> & values)
	{
		stringstream logStream;

		logStream << getTimestamp() << " manual " << sensorName << " {" << endl;

		for(const auto& kv: values)
		{
			logStream << "\t" << kv.first << ":" << kv.second << endl;
		}

		logStream << "}" << endl;

		logString(logStream.str());
	}

	void TwirreLogger::logActuatorEvent(Actuator * actuator, std::map<std::string, Parameter *> actuatorParameters)
	{
		stringstream logStream;

		logStream << getTimestamp() << " actuate " << actuator->getName() << " {" << endl;

		logStream << logDeviceValues(actuatorParameters);

		logStream << "}" << endl;

		logString(logStream.str());
	}

	void TwirreLogger::logString(const string & str)
	{
		//push to log queue
		{
			lock_guard<mutex> logfileLock(_logfileMutex);
			_logQueue.push_back(str);
		}
		_logfileCV.notify_one();
	}

	void TwirreLogger::logBinArrayValue(Value * val)
	{
		auto dataptr = val->getBuffer();
		size_t nbytes = val->getSize() * val->getElementSize();
		_binaryDataOffset += nbytes;

		//push to queue
		{
			lock_guard<mutex> binfileLock(_binfileMutex);

			size_t start = _binQueueSize;
			_binQueueSize += nbytes;

			_binQueue.reserve(_binQueueSize); //ensure queue has enough capacity
			memcpy(_binQueue.data() + start, dataptr, nbytes);
		}
		_binfileCV.notify_one();
	}

	void TwirreLogger::onDevicelistChanged()
	{
		stringstream logStream;
		logStream << "# TwirreLink device list changed" << endl;
		logStream << getTimestamp() << "devicelist_changed" << endl;
		logString(logStream.str());
	}

	uint64_t TwirreLogger::getTimestamp(void)
	{
		auto now = std::chrono::steady_clock::now();
		auto timestamp = now - _tp_start;
		return std::chrono::duration_cast<std::chrono::microseconds>(timestamp).count();
	}

	void TwirreLogger::setMaxArraySize(size_t max)
	{
		_maxArraySize = max;
	}

	void TwirreLogger::logfileThreadMain()
	{
		std::vector<string> currentData;

		while(true)
		{
			{
				unique_lock<mutex> logfileLock(_logfileMutex);
				_logfileCV.wait(logfileLock, [this](){ return (!_runLogfileThread) || (!_logQueue.empty()); });
				if(!_runLogfileThread) break;

				//fetch new data
				std::swap(_logQueue, currentData);
			}

			//write all data to log file
			for(string & str : currentData)
			{
				*_logfile << str;
			}
			currentData.clear();
		}
	}

	void TwirreLogger::binfileThreadMain()
	{
		std::vector<char> currentData;
		size_t dataSize;

		while(true)
		{
			{
				unique_lock<mutex> binfileLock(_binfileMutex);
				_binfileCV.wait(binfileLock, [this](){ return (!_runBinfileThread) || (_binQueueSize > 0); });
				if(!_runBinfileThread) break;

				//fetch new data
				std::swap(_binQueue, currentData);
				dataSize = _binQueueSize;
				_binQueueSize = 0;
			}

			//write data to logfile
			_binfile->write(currentData.data(), dataSize);
		}
	}
}

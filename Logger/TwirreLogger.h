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

#ifndef LOGGER_TWIRRELOGGER_H_
#define LOGGER_TWIRRELOGGER_H_

#include "../Core/Actuator.h"
#include "../Core/Sensor.h"

#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <numeric>
#include <queue>

namespace twirre
{
	class TwirreLogger
	{
	public:
		TwirreLogger(const std::string & logpath, const std::string & binpath);
		virtual ~TwirreLogger();

		void logActuators(std::map<std::string, Actuator*>& actuators);
		void logSensors(std::map<std::string, Sensor*>& sensors);
		void logSensorEvent(Sensor * sensor, std::map<std::string, Value *> sensorValues);
		void manualSensorEvent(std::string sensorName, const std::vector<std::pair<std::string, std::string>> & values);
		void logActuatorEvent(Actuator * actuator, std::map<std::string, Parameter *> actuatorParameters);

		//callback for twirrelink
		void onDevicelistChanged(void);

		//get current log timestamp
		uint64_t getTimestamp(void);

		//arrays with more than max elements will not have their data written to the binfile
		void setMaxArraySize(size_t max);

	private:
		std::shared_ptr<std::ofstream> _logfile;
		std::shared_ptr<std::ofstream> _binfile;
		std::chrono::time_point<std::chrono::steady_clock> _tp_start;

		size_t _binaryDataOffset = 0; //offset where the next binary blob should be written in the binfile
		size_t _maxArraySize = std::numeric_limits<size_t>::max();	//arrays with size above this value are not written to the binfile

		template<class T>
		std::string logDeviceValues(const std::map<std::string, T *> & values);

		void logString(const std::string & str);
		void logBinArrayValue(Value * val);

		//async thread
		std::vector<std::string> _logQueue;
		std::vector<char> _binQueue;
		std::size_t _binQueueSize = 0;	//_binQueue is used as dynamic array, so vector::size() will not work. This means we have to keep track of array size ourselves.
		std::mutex _logfileMutex;
		std::mutex _binfileMutex;
		std::condition_variable _logfileCV;
		std::condition_variable _binfileCV;
		std::thread * _logfileThread;
		std::thread * _binfileThread;

		bool _runLogfileThread = false;
		bool _runBinfileThread = false;

		void logfileThreadMain();
		void binfileThreadMain();

	};
}
#endif /* LOGGER_TWIRRELOGGER_H_ */

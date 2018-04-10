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

#ifndef TWIRRELINK_H_
#define TWIRRELINK_H_

#include <map>
#include <set>
#include <initializer_list>
#include <thread>

#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "Logger/TwirreLogger.h"
#include "DeviceProvider.h"

namespace twirre
{
	/**
	 * Registry for sensors and actuators.
	 */
	class TwirreLink
	{
		friend class DeviceProvider;
	public:
		/**
		 * Create an empty TwirreLink
		 */
		TwirreLink();

		~TwirreLink();

		/**
		 * Create a TwirreLink, immediately adding a DeviceProvider
		 * @param prov the provider to add
		 */
		explicit TwirreLink(DeviceProvider& prov);

		/**
		 * Create a Twirrelink, immediately adding all DeviceProviders
		 * @param provs a vector of pointers to DeviceProviders. May contain nullptrs, which are ignored.
		 */
		TwirreLink(const std::vector<DeviceProvider *> & provs);

		/**
		 * Copy constructor
		 */
		TwirreLink(const TwirreLink & other);

		/**
		 * Move constructor
		 */
		TwirreLink(TwirreLink && other);

		/**
		 * Copy assignment operator
		 */
		TwirreLink& operator = (const TwirreLink & other);

		/**
		 * Move assignment operator
		 */
		TwirreLink& operator = (TwirreLink && other);


		/**
		 * Register all sensors and actuators of the given provider.
		 * The sensor- and actuator-map of the provider will be inserted into the corresponding maps of this TwirreLink.
		 * Name collisions will be resolved by adding "_<number>" to the colliding sensor's or actuator's name.
		 *
		 * @param prov the device provider to register
		 */
		void addProvider(DeviceProvider& prov, bool update = true);

		void removeProvider(DeviceProvider& prov);

		/**
		 * Get all registered actuators
		 *
		 * @return a map, which maps the actuators to their names
		 */
		const std::map<std::string, Actuator*> & getActuators();

		/**
		 * Get all registered sensors
		 *
		 * @return
		 * All sensors are returned inside of a map, which maps them to their names
		 */
		const std::map<std::string, Sensor*> & getSensors();

		/**
		 * Check if a certain sensor exists
		 *
		 * @param sensorName the name of the sensor to check
		 *
		 * @return true if the sensor is registered, false otherwise
		 */
		bool haveSensor(const std::string & sensorName) const;

		/**
		 * Check if a certain actuator exists
		 *
		 * @param actuatorName the name of the actuator to check
		 *
		 * @return true if the actuator is registered, false otherwise
		 */
		bool haveActuator(const std::string & actuatorName) const;

		/**
		 * Get a sensor by name.
		 *
		 * @param sensorName the name of the requested sensor
		 *
		 * @return a reference to the requested sensor
		 *
		 * @throw a runtime_error if no sensor with that name is found
		 */
		Sensor& getSensor(const std::string & sensorName);

		/**
		 * Get an actuator by name.
		 *
		 * @param actuatorName the name of the requested actuator
		 *
		 * @return a reference to the requested actuator
		 *
		 * @throw a runtime_error if no actuator with that name is found
		 */
		Actuator& getActuator(const std::string & actuatorName);

		/**
		 * Start logging all actions to logfile at loggingPath
		 *
		 * @param loggingPath path where the logfile should be created
		 */
		void startLogging(const std::string & loggingPath);

		/**
		 * Stop logging and close the logfile
		 *
		 * @return returns true if a logger was active and has been stopped, returns false if no logger was active.
		 */
		bool stopLogging(void);

		/**
		 * Manually write some values into the logfile, similar to a 'sense' event (except that it will be called a 'manual' event instead)
		 *
		 * @param sensorName The name of the sensor to emulate
		 * @param values A vector containing pairs of valuenames with their corresponding values to write
		 */
		void writeToLog(std::string sensorName, const std::vector<std::pair<std::string, std::string>> & values);

		/**
		 * Set a size limit for logging array values. Has no effect when logging hasn't been started.
		 *
		 * @param max The maximum size of arrays to fully log. Arrays with bigger size are not written to the binfile.
		 */
		void setLoggingMaxArraySize(size_t max);

	private:
		void logDevices(void);
		void notifyChange(void);
		void removeLink(DeviceProvider * which); //called by DeviceProvider's destructor

		std::set<DeviceProvider *> _providers;
		std::map<std::string, Actuator*> _actuatorList;
		std::map<std::string, Sensor*> _sensorList;

		mutable std::recursive_mutex _provMutex;	//protects access / modification of providers
		mutable std::recursive_mutex _tlMutex;	//protects access / modification of sensorlist and acutatorlist

		TwirreLogger * _logger = nullptr;
	};

} /* namespace twirre */

#endif /* TWIRRELINK_H_ */

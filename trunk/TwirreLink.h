/*
 * TwirreLink.h
 *
 *  Created on: Apr 9, 2015
 *      Author: root
 */

#ifndef TWIRRELINK_H_
#define TWIRRELINK_H_

#include <map>
#include <set>
#include <initializer_list>

#include "Core/Actuator.h"
#include "Core/Sensor.h"
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

	private:
		void notifyChange();
		void removeLink(DeviceProvider * which); //called by DeviceProvider's destructor

		std::set<DeviceProvider *> _providers;
		std::map<std::string, Actuator*> _actuatorList;
		std::map<std::string, Sensor*> _sensorList;
	};

} /* namespace twirre */

#endif /* TWIRRELINK_H_ */

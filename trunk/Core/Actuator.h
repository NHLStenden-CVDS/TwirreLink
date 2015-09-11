/*
 * Actuator.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */
#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include <mutex>

#include "Device.h"
#include "owned_mutex.h"

namespace twirre
{
	class Actuator: public Device
	{
	public:
		/**
		 * Create an actuator
		 * @param name
		 * @param description
		 */
		Actuator(const std::string name, const std::string description);
		virtual ~Actuator();

		/*
		 * This class must never be passed by value
		 */
		//disable empty constructor
		Actuator() = delete;
		//disable copy
		Actuator(const Actuator&) = delete;
		//disable move
		Actuator(Actuator&&) = delete;
		//disable assignment
		Actuator& operator=(const Actuator&) = delete;
		//disable move assignment
		Actuator& operator=(Actuator&&) = delete;

		/**
		 * Get the names of all available parameters
		 *
		 * @return an array with all available parameter names
		 */
		std::vector<std::string> getAvailableParameters();

		/**
		 * Check if a parameter exists
		 *
		 * @param name the name of the parameter to check
		 *
		 * @return true if a parameter with that name exists, false otherwise
		 */
		bool haveParameter(std::string name);

		/**
		 * Check if multiple parameters exist
		 *
		 * @param names the names of the parameters to check
		 *
		 * @return true if all names exist, false otherwise
		 */
		bool haveParameters(std::vector<std::string> names);

		/**
		 * Get a parameter by name
		 *
		 * @param name the name of the parameter to get
		 *
		 * @return a reference to the requested parameter (if name is valid), a reference to ErrorValue otherwise
		 */
		virtual Parameter & GetParameter(const std::string & name);
		//this forwards to GetParameter(name)
		virtual Parameter & operator[](const std::string & name);

		void Actuate();

	protected:
		std::map<std::string, Parameter*> _parametersList;
		owned_mutex _actuateMutex; //lock actuator from first value set until actuation
		void registerParameter(Parameter* val);

		/**
		 * Update this Actuator using the modified parameters
		 */
		virtual void ActuateImpl() = 0;
	};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

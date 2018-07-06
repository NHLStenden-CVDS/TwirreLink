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

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include <mutex>

#include "Device.h"
#include "owned_mutex.h"

namespace twirre
{
	/**
	 * Actuator is the base class for all 'actuator'-type devices.
	 *
	 *
	 *
	 * Actuator is thread-safe in such a way that multiple threads can interact with it without requiring external
	 * synchronisation. The Actuator will never see an inconsistent/mixed state of parameters.
	 *
	 * This safety is implemented using an 'owned_mutex', which keeps track of which thread has currently locked it.
	 * Upon setting the first parameter, the thread will take 'ownership' of the mutex, or, if the mutex is already
	 * owned by a different thread, wait until ownership is released. Once a thread has ownership, it is free to set
	 * additional parameters as required. Upon calling the Actuate() function, ownership is released allowing another
	 * thread to interact with the actuator.
	 *
	 * Essentially, the Actuator will be locked by a specific thread between setting the first parameter and calling
	 * the Actuate() function. Because of this, it is recommended to keep the delay between setting parameters and
	 * calling Actuate() as short as possible.
	 */
	class Actuator: public Device
	{
	public:
		/**
		 * Create an actuator
		 * @param name the name this instance will get in TwirreLink
		 * @param description a short description of the type of device this actuator represents
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
		/**
		 * this forwards to GetParameter(name)
		 * @see GetParameter()
		 */
		virtual Parameter & operator[](const std::string & name);

		/**
		 * Get multiple parameters by name
		 *
		 * @param names the names of the parameters to get
		 *
		 * @return a map of names to parameter-pointers is returned. For each requested name, an entry will be present. If no
		 * parameter exists with a given name, that name will be mapped to a pointer to ErrorValue in the result.
		 */
		virtual std::map<std::string, Parameter*> GetParameters(const std::vector<std::string> & names);

		/**
		 * Apply the updated parameters.
		 *
		 * The Actuator will get notified that one or more parameters have been updated, giving it the opportunity to react on those changes
		 */
		void Actuate();

		void clearActuateLoggerCallback();
		void setActuateLoggerCallback(std::function<void(Actuator *, std::map<std::string, Parameter*>&)> cbfn);

	protected:
		std::map<std::string, Parameter*> _parametersList;
		owned_mutex _actuateMutex; //lock actuator from first value set until actuation
		void registerParameter(Parameter* val);
		void registerParameters(std::vector<Parameter *> parms);

		std::function<void(Actuator *, std::map<std::string, Parameter*>&)> _actuateLoggerCallback;
		bool _actuateLoggerSet;

		void logActuation();

		/**
		 * Update this Actuator using the modified parameters
		 *
		 * This function must be implemented by a derived class. It will be called inside of the Actuate() function.
		 */
		virtual void ActuateImpl() = 0;
	};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

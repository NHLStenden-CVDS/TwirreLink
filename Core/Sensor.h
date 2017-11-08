 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
  */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <vector>

#include "Device.h"

namespace twirre
{
	class Sensor: public Device
	{
	public:
		Sensor(const std::string name, const std::string description);
		virtual ~Sensor();

		/*
		 * This class must never be passed by value
		 */
		//disable empty constructor
		Sensor() = delete;
		//disable copy
		Sensor(const Sensor&) = delete;
		//disable move
		Sensor(Sensor&&) = delete;
		//disable assignment
		Sensor& operator=(const Sensor&) = delete;
		//disable move assignment
		Sensor& operator=(Sensor&&) = delete;

		virtual std::map<std::string, Value*> SenseAll();

		virtual std::vector<std::string> getAvailableValues();
		virtual bool haveValue(std::string name);
		virtual bool haveValues(std::vector<std::string>);

		virtual Value & peekValue(std::string name);
		virtual std::map<std::string, Value*> peekValues(const std::vector<std::string> &names);


		virtual std::map<std::string, Value*> Sense_impl(const std::vector<std::string> &names);

		virtual void onSense(const std::vector<std::string> &names);	//will be called by the Sense functions before sense_impl is called
		Value & Sense(const std::string &name);
		std::map<std::string, Value*> Sense(const std::vector<std::string> &names);

		Value & operator[](const std::string &name);
		std::map<std::string, Value*> operator[](const std::vector<std::string> &names);

		virtual std::string ToString() override;

		void clearLoggerCallback();
		void setLoggerCallback(std::function<void(Sensor *, std::map<std::string, Value*>&)> cbfn);
	protected:
		std::function<void(Sensor *, std::map<std::string, Value*>&)> _loggerCallback;
		std::map<std::string, Value*> _valueList;
		void registerValue(Value* val);
		void registerValues(std::vector<Value *> vals);
	};

} /* namespace twirre */

#endif /* SENSOR_H_ */

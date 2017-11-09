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

#ifndef TWIRRESERIAL_H_
#define TWIRRESERIAL_H_

#include <vector>
#include <string>
#include <mutex>

#include "../Core/Actuator.h"
#include "../Core/Sensor.h"
#include "../Core/Value.h"

#include "../Serial/SerialRW.h"

#include "../DeviceProvider.h"

namespace twirre
{
class TwirreSerial : public DeviceProvider
{
	friend class SerialSensor;
	friend class SerialActuator;

public:
	explicit TwirreSerial(const char* path, const int baudrate = 115200);
	~TwirreSerial();

	bool Ping();

private:
	static bool CheckOk(SerialRW & serialRW);

	std::map<std::string, Actuator*> _actuatorList;
	std::map<std::string, Sensor*> _sensorList;
	SerialRW _serial;
	std::mutex _serialMutex; //mutex for all post-init serial actions

	template<typename T, typename V> bool _ProcessInitString(const std::string & s, std::map<std::string, T*> &deviceList);
	bool _InitActuators();
	bool _InitSensors();
	std::map<std::string, Value*> _ProcessValuesString(const std::string & s);

	virtual const std::map<std::string, Actuator*> & getActuators() override;
	virtual const std::map<std::string, Sensor*> & getSensors() override;

};

} /* namespace twirre */

#endif /* TWIRRESERIAL_H_ */

/*
 * DeviceProvider.h
 *
 *  Created on: Apr 9, 2015
 *      Author: root
 */

#ifndef DEVICEPROVIDER_H_
#define DEVICEPROVIDER_H_

namespace twirre
{
	class DeviceProvider
	{
		friend class TwirreLink;
	public:
		DeviceProvider() {};
		virtual ~DeviceProvider() {};
		virtual const std::map<std::string, Actuator*> & getActuators() = 0;
		virtual const std::map<std::string, Sensor*> & getSensors() = 0;
	};

} /* namespace twirre */

#endif /* DEVICEPROVIDER_H_ */

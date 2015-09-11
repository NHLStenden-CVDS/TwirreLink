/*
 * DeviceProvider.h
 *
 *  Created on: Apr 9, 2015
 *      Author: root
 */

#ifndef DEVICEPROVIDER_H_
#define DEVICEPROVIDER_H_

#include <map>
#include <set>

#include "Core/Actuator.h"
#include "Core/Sensor.h"
#include "Core/Value.h"

namespace twirre
{
	class TwirreLink;

	class DeviceProvider
	{
		friend class TwirreLink;
	public:
		DeviceProvider() {};
		virtual ~DeviceProvider();
		virtual const std::map<std::string, Actuator*> & getActuators() = 0;
		virtual const std::map<std::string, Sensor*> & getSensors() = 0;
	protected:
		void doNotifyChange();
	private:
		std::set<TwirreLink *> _links;
		void addLink(TwirreLink * link);
		void removeLink(TwirreLink * link);
	};

} /* namespace twirre */

#endif /* DEVICEPROVIDER_H_ */

/*
 * DeviceProvider.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: m-r-d
 */

#include "TwirreLink.h"

#include "DeviceProvider.h"


namespace twirre
{
	DeviceProvider::~DeviceProvider()
	{
		for(auto link : _links)
		{
			link->removeLink(this);
		}
		doNotifyChange();
	}

	void DeviceProvider::doNotifyChange()
	{
		//notify all links that something has changed
		for(auto link : _links)
		{
			link->notifyChange();
		}
	}

	void DeviceProvider::addLink(TwirreLink * link)
	{
		_links.insert(link);
	}

	void DeviceProvider::removeLink(TwirreLink * link)
	{
		_links.erase(link);
	}
}

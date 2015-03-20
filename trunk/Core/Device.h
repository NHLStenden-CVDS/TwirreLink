/*
 * Device.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include <string>
#include <map>
#include "Value.h"

#ifndef DEVICE_H_
#define DEVICE_H_

namespace twirre
{
	class Device
	{
		public:
			Device();

			int						ID;
			std::string 			Name;
			std::string 			Description;

			std::string 			ToString(void);
	};

} /* namespace twirre */

#endif /* DEVICE_H_ */

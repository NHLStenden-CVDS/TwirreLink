/*
 * Actuator.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#include "Device.h"

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

namespace twirre
{
	class Actuator : public Device
	{
		public:
			Actuator ();

			std::map<std::string, Value> 	GetParameters (void);
			void 							Actuate (std::map<std::string, Value>);

		private:
			std::map<std::string, Value> 	DefaultParameters;
	};

} /* namespace twirre */

#endif /* ACTUATOR_H_ */

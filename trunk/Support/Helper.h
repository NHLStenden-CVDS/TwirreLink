/*
 * Helper.h
 *
 *  Created on: Mar 10, 2015
 *      Author: root
 */

#include <vector>
#include <string>

#ifndef SUPPORT_HELPER_H_
#define SUPPORT_HELPER_H_

namespace twirre
{

	class Helper
	{
		public:
			static void split(const std::string&, char, std::vector<std::string>&);
	};

} /* namespace twirre */

#endif /* SUPPORT_HELPER_H_ */

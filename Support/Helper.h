 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
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

/*
 * Helper.cpp
 *
 *  Created on: Mar 10, 2015
 *      Author: root
 */

#include "Helper.h"

namespace twirre
{
	void Helper::split (const std::string& s, char c, std::vector<std::string>& v)
	{
		std::string::size_type i = 0;
		std::string::size_type j = s.find(c);

		while (j != std::string::npos)
		{
			v.push_back(s.substr(i, j - i));
			i = ++j;
			j = s.find(c, j);

			if (j == std::string::npos)
			{
				v.push_back(s.substr(i, s.length()));
			}
		}
	}
} /* namespace twirre */

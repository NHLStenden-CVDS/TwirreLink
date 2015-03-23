/*
 * Value.h
 *
 *  Created on: Mar 6, 2015
 *      Author: root
 */

#ifndef VALUE_H_
#define VALUE_H_

#include <string>

using namespace std;

namespace twirre
{
class Value
{
public:
	Value(int ID, string name, string type);
	int ID;
	string Name;
	int GetSize();
	void SetBuffer(unsigned char *buffer);
};

} /* namespace twirre */

#endif /* VALUE_H_ */

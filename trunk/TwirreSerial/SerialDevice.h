/*
 * SerialDevice.h
 *
 *  Created on: Apr 23, 2015
 *      Author: jasper
 */

#ifndef SERIALDEVICE_H_
#define SERIALDEVICE_H_

namespace twirre
{
	class SerialDevice
	{
	public:
		int getId() const
		{
			return _id;
		}
	protected:
		explicit SerialDevice(const int id) : _id(id) {};
		virtual ~SerialDevice() {};
		const int _id;
	};
}



#endif /* SERIALDEVICE_H_ */

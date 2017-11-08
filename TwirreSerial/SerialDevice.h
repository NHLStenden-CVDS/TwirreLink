 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
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

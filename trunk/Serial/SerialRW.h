/*
 * SerialRW.h
 *
 *  Created on: Dec 20, 2012
 *      Author: jasper
 */

#ifndef SERIALRW_H_
#define SERIALRW_H_

class SerialRW
{
public:
	SerialRW();
	~SerialRW();
	int _fd;
	int Initialize(char *serialPort, int baud);
	unsigned char readByte();
	std::string readString();
	void readNBytes(unsigned char *buf, int n);
	void writeBytes(unsigned char *bytes, int nrOfBytes);
	void flush();

	template<typename T>
	void Write(T thing)
	{
		writeBytes((unsigned char*)&thing, sizeof(thing));
	}

	template<typename T>
	T Read()
	{
		T thing;
		readNBytes((unsigned char*)&thing, sizeof(thing));
		return thing;
	}

private:
	int bytesToRead;
	void _changeNrOfBytesNeeded(int nrOfBytesNeeded);
	bool _CheckFdTimeout(int usec);
};

#endif /* SERIALRW_H_ */

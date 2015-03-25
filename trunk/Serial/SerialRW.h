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
	int Initialize(const char *serialPort, int baud);
	unsigned char readByte();
	bool readString(std::string &s);
	int readNBytes(unsigned char *buf, int n);
	int writeBytes(unsigned char *bytes, int nrOfBytes);
	void flush();

	template<typename T>
	int Write(T thing)
	{
		return writeBytes((unsigned char*)&thing, sizeof(T));
	}

	template<typename T>
	int Read(T &buffer)
	{
		return readNBytes((unsigned char*)&buffer, sizeof(T));
	}

private:
	int bytesToRead;
	void _changeNrOfBytesNeeded(int nrOfBytesNeeded);
	bool _CheckFdTimeout(int usec);
};

#endif /* SERIALRW_H_ */

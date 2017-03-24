/*
 * SerialRW.h
 *
 *  Created on: Dec 20, 2012
 *      Author: jasper
 */

#ifndef SERIALRW_H_
#define SERIALRW_H_

enum class StringTerminator
{
	ZERO,
	LF
};

class SerialRW
{
public:
	SerialRW();
	~SerialRW();
	int _fd;
	int Initialize(const char *serialPort, int baud);
	bool readString(std::string &s, StringTerminator term = StringTerminator::ZERO);
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

	template<typename T>
	T Read(void)
	{
		T thing;
		readNBytes((unsigned char*)&thing, sizeof(T));
		return thing;
	}

private:
	bool _CheckFdTimeout(int usec);
};

#endif /* SERIALRW_H_ */

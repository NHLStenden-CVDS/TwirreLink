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
	SerialRW(char *serialPort, int baud);
	~SerialRW();
	int _fd;
	int Initialize(char *serialPort, int baud);
	unsigned char readByte();
	std::string readString();
	void readNBytes(unsigned char *buf, int n);
	short readShort();
	unsigned short readUShort();
	void writeBytes(unsigned char *bytes, int nrOfBytes);
	int writeByte(unsigned char byte);
	void writeShort(short nShort);
	void writeUShort(unsigned uShort);
	void writeFloat(float value);
	void flush();
	short _twoBytesToShort(unsigned char msb, unsigned char lsb);
	float _fourBytesToFloat(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);

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
};

#endif /* SERIALRW_H_ */

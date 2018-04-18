/*
 * Twirre: architecture for autonomous UAVs using interchangeable commodity components
 *
 * Copyright© 2017 Centre of expertise in Computer Vision & Data Science, NHL Stenden University of applied sciences
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
	union {
		int _fd;
		void* _handle;
	};
	bool Initialize(const char *serialPort, int baud);
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

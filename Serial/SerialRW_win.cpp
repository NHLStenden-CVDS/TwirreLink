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

#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <sstream>

#include <windows.h>

#include "SerialRW.h"

using namespace std;


const int READ_TIMEOUT_FIRSTBYTE = 1 * 1000;		//time in microseconds to wait for incoming data
const int READ_TIMEOUT_INTRABYTE = 400;					//time in deciseconds to wait for additional bytes (resets after each received byte)

#define BAUD_CASE(RATE)							\
		case RATE:								\
			brate = B##RATE ;					\
			break;



SerialRW::SerialRW()
{
}

bool SerialRW::Initialize(const char *serialPort, int baud)
{
	return _ser.Open(3, 115200);
}

void SerialRW::flush()
{
	int buf;
	int rd = 1;
	while (rd)
	{
		rd = _ser.ReadData(&buf, 4);
	}
}



bool SerialRW::readString(std::string &s, StringTerminator term)
{
	std::stringstream stringStream;

	char termchar;
	switch (term)
	{
	case StringTerminator::ZERO:
		termchar = '\0';
		break;
	case StringTerminator::LF:
		termchar = '\n';
		break;
	default:
		termchar = '\0';
		break;
	}

	char t;
	if (!Read<char>(t))
	{
		return false;
	}
	while (t != termchar)
	{
		stringStream << t;
		if (!Read<char>(t))
		{
			return false;
		}
	}

	s = stringStream.str();

	return true;
}

//Returns number of bytes read
int SerialRW::readNBytes(unsigned char *buf, int n)
{
	int nread = 0;
	while (nread < n)
	{
		nread += _ser.ReadData(buf + nread, n - nread);
	}
	return nread;
}

int SerialRW::writeBytes(unsigned char *bytes, int nrOfBytes)
{
	return _ser.SendData(reinterpret_cast<const char*>(bytes), nrOfBytes);
}

SerialRW::~SerialRW()
{
	_ser.Close();
}

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



SerialRW::SerialRW() : _handle(nullptr)
{
}

bool SerialRW::Initialize(const char *serialPort, int baud)
{
	_handle = CreateFile(serialPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (_handle == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND) 
		{
			throw std::runtime_error("Failed to find port");
		}
		else 
		{
			throw std::runtime_error("Cannot open port");
		}
	}

	DCB parms = { 0 };
	parms.DCBlength = sizeof(parms);
	parms.BaudRate = baud;
	parms.ByteSize = 8;
	parms.Parity = 0;
	parms.StopBits = 1;
	if (!SetCommState(_handle, &parms)) 
	{
		throw std::runtime_error("Cannot set port attributes");
	}

	COMMTIMEOUTS time = { 0 };
	time.ReadIntervalTimeout = READ_TIMEOUT_INTRABYTE;
	time.ReadTotalTimeoutConstant = READ_TIMEOUT_FIRSTBYTE;
	time.ReadTotalTimeoutMultiplier = READ_TIMEOUT_INTRABYTE;
	time.WriteTotalTimeoutConstant = READ_TIMEOUT_FIRSTBYTE;
	time.WriteTotalTimeoutMultiplier = READ_TIMEOUT_INTRABYTE;
	if (SetCommTimeouts(_handle, &time) == 0)
	{
		throw std::runtime_error("Cannot set port timeouts");
	}

	return true;
}

void SerialRW::flush()
{
	PurgeComm(_handle, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
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
		DWORD read_bytes;
		bool status = ReadFile(_handle, buf + nread, n - nread, &read_bytes, nullptr);
		nread += read_bytes;
		if (!status) break;
	}
	return nread;
}

int SerialRW::writeBytes(unsigned char *bytes, int nrOfBytes)
{
	DWORD written_bytes;
	WriteFile(_handle, bytes, nrOfBytes, &written_bytes, nullptr);
	return written_bytes;
}

bool SerialRW::_CheckFdTimeout(int usec) 
{
	throw std::logic_error("unsupported on Windows");
}

SerialRW::~SerialRW()
{
	CloseHandle(_handle);
}

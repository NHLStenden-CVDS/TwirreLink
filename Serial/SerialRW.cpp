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
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <sstream>

#include "SerialRW.h"

using namespace std;


const int READ_TIMEOUT_FIRSTBYTE = 1 * 1000 * 1000;		//time in microseconds to wait for incoming data
const int READ_TIMEOUT_INTRABYTE = 4;					//time in deciseconds to wait for additional bytes (resets after each received byte)

#define BAUD_CASE(RATE)							\
		case RATE:								\
			brate = B##RATE ;					\
			break;



SerialRW::SerialRW() : _fd(0)
{
}

bool SerialRW::Initialize(const char *serialPort, int baud)
{
	struct termios toptions;

	_fd = open(serialPort, O_RDWR | O_NOCTTY);
	if (_fd == -1)
	{
		throw(runtime_error(string("init_serialport: Unable to open port")));
		return -1;
	}

	if (tcgetattr(_fd, &toptions) < 0)
	{
		throw(runtime_error(string("init_serialport: Couldn't get term attributes")));
		return -1;
	}

	/* set baudrate */
	speed_t brate = baud; // let you override switch below if needed
	switch (baud)
	{
		BAUD_CASE(0)
		BAUD_CASE(50)
		BAUD_CASE(75)
		BAUD_CASE(110)
		BAUD_CASE(134)
		BAUD_CASE(150)
		BAUD_CASE(200)
		BAUD_CASE(300)
		BAUD_CASE(600)
		BAUD_CASE(1200)
		BAUD_CASE(1800)
		BAUD_CASE(2400)
		BAUD_CASE(4800)
		BAUD_CASE(9600)
		BAUD_CASE(19200)
		BAUD_CASE(38400)
		BAUD_CASE(57600)
		BAUD_CASE(115200)
		BAUD_CASE(230400)
		BAUD_CASE(460800)
		BAUD_CASE(500000)
		BAUD_CASE(576000)
		BAUD_CASE(921600)
		BAUD_CASE(1000000)
		BAUD_CASE(1152000)
		BAUD_CASE(1500000)
		BAUD_CASE(2000000)
		BAUD_CASE(2500000)
		BAUD_CASE(3000000)
		BAUD_CASE(3500000)
		BAUD_CASE(4000000)
#ifdef B14400
			BAUD_CASE(14400)
#endif
#ifdef B28800
			BAUD_CASE(28800)
#endif

	default:
		throw std::runtime_error("invalid baud rate selected");


	}
	cfsetispeed(&toptions, brate);
	cfsetospeed(&toptions, brate);

	// 8N1
	toptions.c_cflag &= ~PARENB;
	toptions.c_cflag &= ~CSTOPB;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8;

	// no flow control
	toptions.c_cflag &= ~CRTSCTS;
	toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines

	//this does something
	toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw

	/* set input/output processing flags */
	toptions.c_iflag = 0; //turn off ALL the stuff
	toptions.c_oflag = 0; //turn off EVEN MORE of ALL the stuff

	/* set bytes to read */
	// see: http://unixwiz.net/techtips/termios-vmin-vtime.html
	toptions.c_cc[VMIN] = 0;
	toptions.c_cc[VTIME] = READ_TIMEOUT_INTRABYTE;

	/* commit flags */
	if (tcsetattr(_fd, TCSANOW, &toptions) < 0)
	{
		throw(runtime_error(string("init_serialport: Couldn't set term attributes")));
		return -1;
	}

	return (_fd >= 0);
}

void SerialRW::flush()
{
	tcflush(_fd, TCIOFLUSH);
}



bool SerialRW::readString(std::string &s, StringTerminator term)
{
	std::stringstream stringStream;

	char termchar;
	switch(term)
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
	if(!Read<char>(t))
	{
		return false;
	}
	while (t != termchar)
	{
		stringStream << t;
		if(!Read<char>(t))
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
	while(nread < n)
	{
		if(_CheckFdTimeout(READ_TIMEOUT_FIRSTBYTE))
		{
			nread += read(_fd, buf + nread, n - nread);
		}
		else
		{
			return nread;
		}
	}
	return nread;
}

int SerialRW::writeBytes(unsigned char *bytes, int nrOfBytes)
{
	return write(_fd, bytes, nrOfBytes);
}

bool SerialRW::_CheckFdTimeout(int usec) {
	// Initialize file descriptor sets
	fd_set read_fds, write_fds, except_fds;


	//set timeout
	struct timeval timeout;
	timeout.tv_sec = (usec / 1000000);
	timeout.tv_usec = usec - (timeout.tv_sec * 1000000);

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&except_fds);

	FD_SET(_fd, &read_fds);

	//return 0 if timeout, -1 if error
	int res = select(_fd + 1, &read_fds, &write_fds, &except_fds, &timeout);
	if (res < 0) {
		throw(runtime_error(string("Error occurred in select()")));
	} else {
		return res == 1;
	}
}

SerialRW::~SerialRW()
{
	close(_fd);
}

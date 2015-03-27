/*
 * SerialRW.cpp
 *
 *  Created on: Dec 20, 2012
 *      Author: jasper
 */

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <cstdint>

#include "SerialRW.h"

using namespace std;

SerialRW::SerialRW() : _fd(0), bytesToRead(1)
{
}

int SerialRW::Initialize(const char *serialPort, int baud)
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
		case 4800:
			brate = B4800;
			break;
		case 9600:
			brate = B9600;
			break;
#ifdef B14400
			case 14400: brate=B14400; break;
#endif
		case 19200:
			brate = B19200;
			break;
#ifdef B28800
			case 28800: brate=B28800; break;
#endif
		case 38400:
			brate = B38400;
			break;
		case 57600:
			brate = B57600;
			break;
		case 115200:
			brate = B115200;
			break;
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
	toptions.c_cc[VMIN] = 16;
	bytesToRead = 16;
	toptions.c_cc[VTIME] = 0;

	/* commit flags */
	if (tcsetattr(_fd, TCSANOW, &toptions) < 0)
	{
		throw(runtime_error(string("init_serialport: Couldn't set term attributes")));
		return -1;
	}

	return _fd;
}

void SerialRW::flush()
{
	tcflush(_fd, TCIOFLUSH);
}



bool SerialRW::readString(std::string &s)
{
	s = "";
	s.reserve(2048);

	char t;

	if(!Read<char>(t))
	{
		return false;
	}
	while (t != '\0')
	{
		s += t;
		if(!Read<char>(t))
		{
			return false;
		}
	}

	return true;
}

//Returns number of bytes read
int SerialRW::readNBytes(unsigned char *buf, int n)
{
	if(bytesToRead != n)
	{
	_changeNrOfBytesNeeded(n);
	}

	if(_CheckFdTimeout(250000))
	{
		return read(_fd, buf, n);
	}
	else
	{
		return 0;
	}
}

int SerialRW::writeBytes(unsigned char *bytes, int nrOfBytes)
{
	return write(_fd, bytes, nrOfBytes);
}


void SerialRW::_changeNrOfBytesNeeded(int nrOfBytesNeeded)
{
	struct termios toptions;

	if (tcgetattr(_fd, &toptions) < 0)
	{
		throw(runtime_error(string("init_serialport: Couldn't get term attributes")));
	}

	toptions.c_cc[VMIN] = nrOfBytesNeeded;

	if (tcsetattr(_fd, TCSANOW, &toptions) < 0)
	{
		throw(runtime_error(string("init_serialport: Couldn't set term attributes")));
	}
}

bool SerialRW::_CheckFdTimeout(int usec) {
	// Initialize file descriptor sets
	fd_set read_fds, write_fds, except_fds;

	//set timeout
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = usec;

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

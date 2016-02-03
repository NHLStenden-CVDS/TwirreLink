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


const int READ_TIMEOUT = 1 * 1000 * 1000;		//time in microseconds to wait for incoming data

#define BAUD_CASE(RATE)							\
		case RATE:								\
			brate = B##RATE ;					\
			break;



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
	toptions.c_cc[VMIN] = 16;
	bytesToRead = 16;
	toptions.c_cc[VTIME] = 10;

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

	if(_CheckFdTimeout(READ_TIMEOUT))
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

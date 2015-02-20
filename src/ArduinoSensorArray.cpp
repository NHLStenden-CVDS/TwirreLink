//============================================================================
// Name        : ArduinoSensorArray.cpp
// Author      : Jasper J. J. Westra
// Version     : 1.0
// Description : Library for reading arduino data.
//============================================================================

#include <iostream>
#include "SerialRW.h"
#include <unistd.h>

using namespace std;

int main() {
	cout << " !!! Arduino Sensor Array Library || JJJ W !!!" << endl;
	cout << endl << "------------------" << endl;

	SerialRW *srw = new SerialRW("/dev/ttyACM0", 115200);

	sleep(4);

	while (1)
	{
//		srw->writeByte((unsigned char)'S');
//		srw->writeByte((unsigned char)0);
//		srw->writeByte((unsigned char)1);
//		srw->writeByte((unsigned char)0);
//		srw->writeByte((unsigned char)1);
//		cout << (char)srw->readByte() << " ";
//		cout << (int)srw->readByte() << endl;
	}

	return 0;
}

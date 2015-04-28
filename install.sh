#!/bin/bash

LIB_DIR="/usr/local/lib/twirrelink"
INC_DIR="/usr/local/include/twirrelink"

rm -rf $LIB_DIR
rm -rf $INC_DIR

mkdir $LIB_DIR
mkdir $INC_DIR
mkdir $INC_DIR/Core
mkdir $INC_DIR/TwirreSerial
mkdir $INC_DIR/Serial
mkdir $INC_DIR/TwirreLidar

mkdir build/mergelib

cp build/libtwirrelink_raw.a build/mergelib/
cp trunk/TwirreLidar/RPLidar/librplidar_sdk.a build/mergelib/
cp trunk/TwirreLidar/HokuyoLidar/liburg_c.a build/mergelib/

cd build/mergelib

ar -x libtwirrelink_raw.a
ar -x librplidar_sdk.a
ar -x liburg_c.a
ar -qc libtwirrelink.a  *.o 

cd ../..

cp build/mergelib/libtwirrelink.a $LIB_DIR/
cp trunk/*.h $INC_DIR/
cp trunk/Core/*.h $INC_DIR/Core/
cp trunk/TwirreSerial/TwirreSerial.h $INC_DIR/TwirreSerial/
cp trunk/Serial/SerialRW.h $INC_DIR/Serial/
cp trunk/TwirreLidar/TwirreLidar.h $INC_DIR/TwirreLidar/

rm -rf build/mergelib

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

cp Release/libtwirrelink.so $LIB_DIR/
cp *.h $INC_DIR/
cp Core/*.h $INC_DIR/Core/
cp TwirreSerial/TwirreSerial.h $INC_DIR/TwirreSerial/
cp Serial/SerialRW.h $INC_DIR/Serial/


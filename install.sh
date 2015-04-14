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

cp build/libtwirrelink.a $LIB_DIR/
cp trunk/*.h $INC_DIR/
cp trunk/Core/*.h $INC_DIR/Core/
cp trunk/TwirreSerial/TwirreSerial.h $INC_DIR/TwirreSerial/
cp trunk/Serial/SerialRW.h $INC_DIR/Serial/


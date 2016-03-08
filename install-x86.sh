#!/bin/bash

LIB_DIR="/usr/local/lib"
INC_DIR="/usr/local/include/twirrelink"

rm -rf $INC_DIR

mkdir $INC_DIR
mkdir $INC_DIR/Core
mkdir $INC_DIR/TwirreSerial
mkdir $INC_DIR/Serial

cp x86/libtwirrelink.so $LIB_DIR/
cp *.h $INC_DIR/
cp Core/*.h $INC_DIR/Core/
cp TwirreSerial/TwirreSerial.h $INC_DIR/TwirreSerial/
cp Serial/SerialRW.h $INC_DIR/Serial/

ldconfig

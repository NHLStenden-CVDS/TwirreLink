#!/bin/bash

LIB_DIR="/usr/aarch64-linux-gnu/lib"
INC_DIR="/usr/aarch64-linux-gnu/include/twirrelink"

rm -rf $INC_DIR

mkdir $INC_DIR
mkdir $INC_DIR/Core
mkdir $INC_DIR/TwirreSerial
mkdir $INC_DIR/Serial
mkdir $INC_DIR/Logger

cp ARM/libtwirrelink.so $LIB_DIR/
cp *.h $INC_DIR/
cp Core/*.h $INC_DIR/Core/
cp Logger/*.h $INC_DIR/Logger/
cp TwirreSerial/TwirreSerial.h $INC_DIR/TwirreSerial/
cp Serial/SerialRW.h $INC_DIR/Serial/

ldconfig

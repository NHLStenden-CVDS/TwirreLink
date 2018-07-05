# TwirreLink

* [TwirreLink library](#twirrelink-library)
* [TwirreSerial](#twirreserial)
* [Building on Linux](#building-on-linux)
* [Building on Windows](#building-on-windows)
* [License](#license)
* [Examples](#examples)


## TwirreLink library
TwirreLink is a C++ library for sensor and actuator communication. It has been designed to be easy-to-use, modular and extendable. The generic structure enables a large amount of devices to be integrated into the TwirreLink system.

## TwirreSerial
TwirreLink includes the TwirreSerial protocol for communication between a computer and an Arduino DUE running TwirreArduino firmware. The protocol specification can be found here: <https://github.com/NHLStenden-CVDS/Twirre/releases/download/doc-alpha/twirreSerialProtocol.pdf>

## Building on Linux
TwirreLink only requires a C++14-capable compiler to be built. An Eclipse CDT project file is included in this repository. This is the recommended method of building TwirreLink:

* Import project into Eclipse (File > Import > Existing Projects into Workspace
* Select correct build profile in eclipse (ARM or x86) -> (Right-click project > Build Configurations > Set active)
* Build project

The syntax highlighting of Eclipse might be broken for this project. This is because Eclipse's indexer defaults to the wrong build profile. To fix this: 
* change the indexer build profile (Right-click project > Properties > C/C++ General > Indexer > select correct profile under 'Build configuration for the indexer').
* rebuild the index (Right-click project > Index > Rebuild)

Please note that TwirreLink uses std::thread, which does require you to link the pthread library in projects using the TwirreLink library.

The Eclipse project is configured to run either the **install-arm.sh** or **install-x86.sh** script after the build. The script will install the library file and header files into the corresponding **/usr/local/...** folders (see script for details). As sudo is required for this script, eclipse uses GKSU to start the script and prompt for a password. In order for this to work, the GKSU package must be installed (eg. **sudo apt install gksu**).

## Building on Windows
For Windows, a Visual Studio 2017 project is provided. It can be found in the **MSVS17** folder. The project will create a new folder, with the library file in **lib** folder and header files in **include** folder.

## License
TwirreLink has been made available under the MIT license (see **LICENSE**).

## Examples
Examples to be added at a later date

pushd %~dp0
cd ..\..\
rd /s /q Include
md Include
cd Include

copy ..\..\*.h .\

md Core
copy ..\..\Core\*.h .\Core

md Logger
copy ..\..\Logger\*.h .\Logger

md Serial
copy ..\..\Serial\*.h .\Serial

md Support
copy ..\..\Support\*.h .\Support

md TwirreSerial
copy ..\..\TwirreSerial\*.h .\TwirreSerial

cd ..
md Lib
copy TwirreLink\x64\Release\TwirreLink.lib .\Lib
popd
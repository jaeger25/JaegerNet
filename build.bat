@echo off

if NOT EXIST ./build (
    echo "Must run bootstrap.bat first"
    goto :eof
)

start %COMSPEC% /k cmake --build build/x86-windows --target install --config Release
start %COMSPEC% /k cmake --build build/x64-windows --target install --config Release

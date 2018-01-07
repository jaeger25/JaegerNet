@echo off

if NOT EXIST ./build (
    echo "Must run bootstrap.bat first"
    goto :eof
)

cmake --build build/x86-windows --target install --config Release -- /m:4 /v:m
cmake --build build/x64-windows --target install --config Release -- /m:4 /v:m
cmake --build build/x86-windows --target install --config Debug -- /m:4 /v:m
cmake --build build/x64-windows --target install --config Debug -- /m:4 /v:m

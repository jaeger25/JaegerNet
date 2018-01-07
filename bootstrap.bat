mkdir build & cd build
mkdir x86-windows & cd x86-windows

cmake -G "Visual Studio 15 2017" "-DCMAKE_TOOLCHAIN_FILE=../../../vcpkg/scripts/buildsystems/vcpkg.cmake" "-DVCPKG_TARGET_TRIPLET=x86-windows-static" "-DCMAKE_INSTALL_PREFIX=../../installed/x86-windows" ../../Src

cd ..
mkdir x64-windows & cd x64-windows

cmake -G "Visual Studio 15 2017 Win64" "-DCMAKE_TOOLCHAIN_FILE=../../../vcpkg/scripts/buildsystems/vcpkg.cmake" "-DVCPKG_TARGET_TRIPLET=x64-windows-static" "-DCMAKE_INSTALL_PREFIX=../../installed/x64-windows" ../../Src

cd ../..
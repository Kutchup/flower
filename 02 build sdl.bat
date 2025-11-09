@echo off
TITLE 02 Build SDL
@echo on

cd SDL
mkdir build
cd build

cmake -DCMAKE_CXX_COMPILER=C:\mingw64\bin\g++.exe -DCMAKE_CC_COMPILER=C:\mingw64\bin\gcc.exe -DCMAKE_C_COMPILER=C:\mingw64\bin\gcc.exe -DCMAKE_MAKE_PROGRAM=C:\mingw64\bin\mingw32-make.exe -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..

cmake --build . --config Release --parallel

ECHO Done.
PAUSE

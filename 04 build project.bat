@echo off
TITLE 04 Build Project

cd ..
mkdir FlowerBuild
cd FlowerBuild
@echo on

cmake -DCMAKE_CXX_COMPILER=C:\mingw64\bin\g++.exe -DCMAKE_CC_COMPILER=C:\mingw64\bin\gcc.exe -DCMAKE_C_COMPILER=C:\mingw64\bin\gcc.exe -DCMAKE_MAKE_PROGRAM=C:\mingw64\bin\mingw32-make.exe -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-mwindows" ..\Flower

cmake --build . --config Release --parallel

ECHO Done.
PAUSE

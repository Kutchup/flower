@echo off
TITLE 03 Install SDL in C:\Program Files (x86) which requires Run as Administrator
@echo on

@REM Absolute path because run as admin resets current working directory

cd C:\Users\Waves\Programs\Flower\SDL
cd build
cmake --install . --config Release

copy "C:\Program Files (x86)\SDL3\bin\SDL3.dll" C:\Users\Waves\Programs\FlowerBuild

ECHO Done.
PAUSE

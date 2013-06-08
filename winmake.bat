@ECHO off
set PATH=%PATH%;C:\MinGW\bin
set PATH=%PATH%;C:\MinGW\msys\1.0\bin
pushd %~dp0
mingw32-make -f Makefile clean
mingw32-make -f Makefile
IF NOT EXIST C:\Windows\System32\glew32.dll (
	xcopy GL4-engine\lib\mingw32\glew32.dll C:\Windows\System32\
)
PAUSE
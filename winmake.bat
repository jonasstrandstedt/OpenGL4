@ECHO off
set PATH=%PATH%;C:\MinGW\bin
set PATH=%PATH%;C:\MinGW\msys\1.0\bin
mingw32-make -f Makefile clean
mingw32-make -f Makefile
PAUSE
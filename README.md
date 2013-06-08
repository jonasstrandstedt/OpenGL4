#OpenGL4

A simple OpenGL 4 engine. The purpose is for me to learn OpenGL4 and create an engine that is easy to use in other projects. The engine does not use OpenGL 4 specific functions due to backwards compatibily for Mac, but the engine makes it easy to include OpenGL 4.

##MIT-Licence
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

##Supported systems
###Linux
Full support when gcc and necessary libraries is installed. I am using Ubuntu 12.10 and have installed the libraries through the package manager.
###Mac OS X
Mac OS X does not currently support OpenGL 4 but the core engine is not using OpenGL 4 specific functions and therefore Mac OS X is using OpenGL 3.2. 
NOTE: The provided blur shader makes the computer lock/crash on Intel HD 3000 and is therefore disabled for Mac users by default.
###Windows
There is windows support using MinGW and Msys installed in "C:/MinGW" and "C:/MinGW" and ""C:/MinGW"/msys" respectively. Use the provided bat files if you do not want to edit the PATH yourself.
NOTE: I have not managed to compile libpng for MinGW and it is therefore disabled by default.
###Libraries used
- GLEW, http://glew.sourceforge.net/
- GLFW, http://www.glfw.org/
- libpng, http://www.libpng.org/pub/png/libpng.html

##Acknowledges
Images of Earth provided by Nasa
http://visibleearth.nasa.gov/
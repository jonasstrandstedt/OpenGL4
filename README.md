#OpenGL4

A simple OpenGL 4 engine. The purpose is for me to learn OpenGL4 and create an engine that is easy to use in other projects.

##MIT-Licence
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

##Supported systems
###Linux
Full support when gcc and necessary libraries is installed. I am using Ubuntu 12.04 and have installed the libraries through the package manager.
###Mac OS X
Mac OS X does not currently support OpenGL4 but the features used should be able to port to Mac by using OpenGL 3.2 and GLSL 1.50. I am currently trying to enable the engine to work with Opengl version < 4.3.
###Windows
I am not using Windows myself so porting feels a bit tricky. I have looked into it but with no success, I tried with mingw but was not able to compile all libraries. I will try using Visual Studio in the future.
###Libraries used
- GLEW, http://glew.sourceforge.net/
- GLFW, http://www.glfw.org/
- libpng, http://www.libpng.org/pub/png/libpng.html

##Acknowledges
Images of Earth provided by Nasa
http://visibleearth.nasa.gov/
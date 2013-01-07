/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef ENGINE_H
#define ENGINE_H

// GL related libraries
#include <GL/glew.h>
#include <GL/glfw.h>

// std libraries
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GL4-Engine related classes
#include "FBO.h"
#include "VBO.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#define GL4_ENGINE_VERION_MAJOR 0
#define GL4_ENGINE_VERION_MINOR 0
#define GL4_ENGINE_VERION_REVISION 1

enum {
	ENGINE_PRESS = 0,
	ENGINE_RELEASE
};


namespace gl4
{
	class Engine
	{
		public:
			Engine(int argc, char **argv);
			~Engine();

			bool initGL();
			void render();

			// set callback functions
			void setKeyBoardCallbackfunc(void (*f)(int, int));
			void setUpdateFunc(void (*f)(float));
			void setRenderFunc(void (*f)(void));
			void setInitFunc(void (*f)(void));

			// other set functions
			void setWindowTitle(const char *title);

			// camera functions
			void usePerspectiveProjection(glm::mat4 transform = glm::mat4(1.0));
			void useOrthogonalProjection(glm::mat4 transform = glm::mat4(1.0));

			// input handling
			bool isKeyPressed(int key);


		private:
			void loadTexture(const char *filename, GLuint texID);

			// user defined functions
			void (*_keyboardCallbackFunc)(int, int);
			void (*_updateFunc)(float);
			void (*_renderFunc)(void);
			void (*_initFunc)(void);

			//variables setting the speed
			double _t0;

			// variables
			glm::mat4 _perspectiveProjectionMatrix;
			glm::mat4 _orthogonalProjectionMatrix;
			bool _doRender;
			unsigned int _windowWidth;
			unsigned int _windowHeight;

			// FBOs
			FBO _standard;
			VBO _quad;
			GLuint textureID;

			// private functions
			void _stopRender();
	};
}

#endif
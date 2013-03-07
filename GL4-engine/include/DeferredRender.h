/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef DEFERREDRENDER_H
#define DEFERREDRENDER_H

// GL related libraries
#include <GL/glew.h>
#include <GL/glfw.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// other libraries
#include <string>

#include "FBO.h"
#include "VBO.h"

enum {
	DEFERRED_WIREFRAME = 0,
	DEFERRED_LIGHTSOURCE,
	DEFERRED_TEXTURE,
};

namespace gl4
{
	class DeferredRender
	{
		public:

			DeferredRender();
			~DeferredRender();

			void init(unsigned int windowWidth, unsigned int windowHeight);
			void addExtendedDeferredShaderFromFile(std::string name, const char *filename = 0, bool pass2 = false);
			void bindDefaultShader();
			void bindShader(std::string shader);
			void enable(int state);
			void disable(int state);
			void useState(int uniform, bool state);
			void render(void (*f)(void));

		private:
			// variables
			unsigned int _windowWidth;
			unsigned int _windowHeight;
			glm::mat4 _orthogonalProjectionMatrix;

			// if's
			bool _wireframe;
			bool _lightsource;

			// FBOs
			FBO _standard;
			VBO _quad;

	};
}

#endif
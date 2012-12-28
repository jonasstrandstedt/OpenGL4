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


		private:
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

			// private functions
			void _stopRender();
	};
}

#endif
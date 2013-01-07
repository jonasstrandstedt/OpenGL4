/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Engine.h"

/*
 * loadTexture - load 8-bit texture data from a TGA file
 * and set up the corresponding texture object.
 */
void gl4::Engine::loadTexture(const char *filename, GLuint texID) {
  
  GLFWimage img; // Use intermediate GLFWimage to get width and height

  if(!glfwReadImage(filename, &img, GLFW_NO_RESCALE_BIT))
  	std::cerr << "Failed to load texture from TGA file." << std::endl;
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture( GL_TEXTURE_2D, texID );

  glfwLoadTextureImage2D( &img, 0 );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glfwFreeImage(&img); // Clean up the malloc()'ed data pointer
}

gl4::Engine::Engine(int argc, char **argv) {
	_doRender = true;

	_keyboardCallbackFunc = 0;
	_updateFunc = 0;
	_renderFunc = 0;
	_initFunc = 0;

	_windowWidth = 800;
	_windowHeight = 600;

	glm::mat4 Perspective = glm::perspective(45.0f, static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight), 0.1f, 100.0f);
	glm::mat4 Orthogonal = glm::ortho(0.0f,static_cast<float>(_windowWidth), 0.0f,static_cast<float>(_windowHeight), 0.1f, 100.0f);
	glm::mat4 PerspectiveView = glm::lookAt(	glm::vec3(0,1,3), // Camera pos
												glm::vec3(0,0,0), // and looks at the origin
												glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
																);
	glm::mat4 OrthogonalView = glm::lookAt(		glm::vec3(0,0,1), // Camera pos
												glm::vec3(0,0,0), // and looks at the origin
												glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
																);

	_perspectiveProjectionMatrix = Perspective * PerspectiveView;
	_orthogonalProjectionMatrix = Orthogonal * OrthogonalView;
}

bool gl4::Engine::initGL() {

	//init glfw and set start time
	glfwInit();
	_t0 = glfwGetTime();

#ifdef __APPLE__
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 2 );
#else
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 4 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 3 );
#endif
	glfwOpenWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );
	
	// open window
	glfwOpenWindow( _windowWidth, _windowHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW );

	// check if window opened correctly
	if( glfwGetWindowParam( GLFW_OPENED ) ) {
		glEnable(GL_TEXTURE_2D); 
		// init GLEW with experimental features
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cerr << "Error " << glewGetErrorString(err) << std::endl;
		}

		// glew init with "glewExperimental = GL_TRUE" fires an error
		GLenum errorID = glGetError();

		// fetching versions
		char buffer [50];
		const char *format = "%-12s %i.%i.%i";
		int GLFWmajor = 0, GLFWminor = 0, GLFWrev = 0;
		int OpenGLmajor = 0, OpenGLminor = 0, OpenGLrev = 0;
		glfwGetVersion( &GLFWmajor, &GLFWminor, &GLFWrev );
		OpenGLmajor = glfwGetWindowParam(GLFW_OPENGL_VERSION_MAJOR);
		OpenGLminor = glfwGetWindowParam(GLFW_OPENGL_VERSION_MINOR);

		// printing versions
		std::cout << "Using versions: " << std::endl;
		std::sprintf (buffer, format, "GLFW:", GLFWmajor, GLFWminor, GLFWrev );
		std::cout << buffer << std::endl;
		std::sprintf (buffer, "%-12s %s", "GLEW:", glewGetString(GLEW_VERSION) );
		std::cout << buffer << std::endl;
		std::sprintf (buffer, format, "OpenGL:", OpenGLmajor, OpenGLminor, OpenGLrev );
		std::cout << buffer << std::endl;
		std::sprintf (buffer, format, "GL4 Engine:", GL4_ENGINE_VERION_MAJOR, GL4_ENGINE_VERION_MINOR, GL4_ENGINE_VERION_REVISION );
		std::cout << buffer << std::endl;
		glfwSetWindowTitle( buffer );

		
		// init shaders
		gl4::ShaderManager::getInstance();

		// init FBO with same size as window with 32 samples/pixel and 3 textures
		_standard.init(_windowWidth, _windowHeight, 32,3);

		// init fullscreen quad
		_quad.init(_windowWidth,_windowHeight);

		// call user defined init 
		if (_initFunc != 0)
		{
			_initFunc();
		}

		// set callbacks
		if (_keyboardCallbackFunc != 0)
		{
			glfwSetKeyCallback(_keyboardCallbackFunc);
		}
		return true;
	}

	// if unable to open window, print error
	std::cerr << "Could not open window" << std::endl;

	return false;
}

gl4::Engine::~Engine() {
	glfwTerminate();
}


void gl4::Engine::setKeyBoardCallbackfunc(void (*f)(int, int)) {
	_keyboardCallbackFunc = f;
}
void gl4::Engine::setRenderFunc(void (*f)(void)) {
	_renderFunc = f;
}
void gl4::Engine::setUpdateFunc(void (*f)(float)) {
	_updateFunc = f;
}
void gl4::Engine::setInitFunc(void (*f)(void)) {
	_initFunc = f;
}
void gl4::Engine::setWindowTitle(const char *title) {
	glfwSetWindowTitle( title );
}

void gl4::Engine::usePerspectiveProjection(glm::mat4 transform)
{
	glUniformMatrix4fv(0, 1, GL_FALSE, &_perspectiveProjectionMatrix[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &transform[0][0]);
}

void gl4::Engine::useOrthogonalProjection(glm::mat4 transform)
{
	glUniformMatrix4fv(0, 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &transform[0][0]);
}

void gl4::Engine::render() {
	while(_doRender) {

		double t = glfwGetTime();
		float dt = static_cast<float>(t - _t0);
		// Handle all the updates
		if (_updateFunc != 0)
		{
			_updateFunc(dt);
		}
		_t0 = t;


		// Clear color buffer
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// render user content to fullscreen FBO to enable post-processing capabilities
		_standard.clear();
		_standard.bind();
		if (_renderFunc != 0)
		{
			_renderFunc();
		}
		_standard.unbind();

		gl4::ShaderManager::getInstance()->bindShader("Deferred2");
		// orthogonal projection
		useOrthogonalProjection();

		// set the camera poisition uniform
		glUniform3f(2, 0,1,3);

		// render the FBO texture
		_standard.bindTextures(3);
		_quad.render();

		// unbind texture and shader
		glBindTexture(GL_TEXTURE_2D,0);
		gl4::ShaderManager::getInstance()->unbindShader();

		// swap buffers
		glfwSwapBuffers();

		// exit if user press ESC
		if ( glfwGetKey( GLFW_KEY_ESC ) == GLFW_PRESS || !glfwGetWindowParam( GLFW_OPENED ) )
			_stopRender();
	}
}


bool gl4::Engine::isKeyPressed(int key)
{
	return glfwGetKey(key) == GLFW_PRESS;
}

void gl4::Engine::_stopRender() {
	_doRender = false;
}

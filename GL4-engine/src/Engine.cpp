#include "Engine.h"
#include "ShaderManager.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

gl4::Engine::Engine(int argc, char **argv) {
	_doRender = true;

	_keyboardCallbackFunc = 0;
	_renderFunc = 0;
	_initFunc = 0;

	int nrofkeys = 256;
	_keyboardKeyPressed = (bool*)std::malloc(nrofkeys*sizeof(bool));
	for (int i = 0; i < nrofkeys; ++i)
	{
		_keyboardKeyPressed[i] = false;
	}

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


	glfwInit();
	
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 4 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 3 );
	glfwOpenWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );
	
	glfwOpenWindow( _windowWidth, _windowHeight, 0, 0, 0, 0, 0, 0, GLFW_WINDOW );
	if( glfwGetWindowParam( GLFW_OPENED ) ) {

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cerr << "Error " << glewGetErrorString(err) << std::endl;
		}

		// glew init with "glewExperimental = GL_TRUE" fires an error
		GLenum errorID = glGetError();

		char buffer [50];
		const char *format = "%-12s %i.%i.%i";
		int GLFWmajor = 0, GLFWminor = 0, GLFWrev = 0;
		int OpenGLmajor = 0, OpenGLminor = 0, OpenGLrev = 0;

		glfwGetVersion( &GLFWmajor, &GLFWminor, &GLFWrev );
		OpenGLmajor = glfwGetWindowParam(GLFW_OPENGL_VERSION_MAJOR);
		OpenGLminor = glfwGetWindowParam(GLFW_OPENGL_VERSION_MINOR);


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

		
		gl4::ShaderManager::getInstance();

		_standard.init(_windowWidth, _windowHeight, 32);
		_quad.init(_windowWidth,_windowHeight);

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
void gl4::Engine::setInitFunc(void (*f)(void)) {
	_initFunc = f;
}
void gl4::Engine::setWindowTitle(const char *title) {
	glfwSetWindowTitle( title );
}

void gl4::Engine::setPerspectiveProjection(glm::mat4 transform)
{
	glm::mat4 finaltransform = _perspectiveProjectionMatrix * transform;
	glUniformMatrix4fv(0, 1, GL_FALSE, &finaltransform[0][0]);
}

void gl4::Engine::setOrthogonalProjection(glm::mat4 transform)
{
	glm::mat4 finaltransform = _orthogonalProjectionMatrix * transform;
	glUniformMatrix4fv(0, 1, GL_FALSE, &finaltransform[0][0]);
}

void gl4::Engine::render() {
	while(_doRender) {

		// render user content to fullscreen FBO to enable post-processing capabilities
		_standard.clear();
		_standard.bind();
		if (_renderFunc != 0)
		{
			_renderFunc();
		}
		_standard.unbind();



		// Clear color buffer
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		gl4::ShaderManager::getInstance()->bindShader("Textured");

		// orthogonal projection
		setOrthogonalProjection();

		// render the FBO texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _standard.getTexture());
		glUniform1i(1, 0);

		_quad.render();

		// unbind texture and shader
		glBindTexture(GL_TEXTURE_2D,0);
		gl4::ShaderManager::getInstance()->unbindShader();

		// swap buffers
		glfwSwapBuffers();

		// exit if user press ESC
		if ( glfwGetKey( GLFW_KEY_ESC ) == GLFW_PRESS )
			_stopRender();
	}
}

void gl4::Engine::_stopRender() {
	_doRender = false;
}
/*
void* gl4::Engine::checkKeyboardInput(void *in)
{
	while( ! shutdown ) {
		usleep(20000);
		for (int i = 0; i < 256; ++i)
		{
			if(_keyboardKeyPressed[i]) {
				if(glfwGetKey( i ) == GLFW_RELEASE) {
					if (_keyboardCallbackFunc != 0)
					{
						_keyboardCallbackFunc(i, ENGINE_RELEASE);
					}
					_keyboardKeyPressed[i] = false;
				}
			} else {
				if(glfwGetKey( i ) == GLFW_PRESS) {
					if (_keyboardCallbackFunc != 0)
					{
						_keyboardCallbackFunc(i, ENGINE_PRESS);
					}
					_keyboardKeyPressed[i] = true;
				}
			}
		}
	}
	free(_keyboardKeyPressed);
	pthread_exit(NULL);
}
*/
/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "DeferredRender.h"
#include "ShaderManager.h"
#include "DeferredShader.h"

const char *deferred1 = "Deferred1";
const char *deferred2 = "Deferred2";


gl4::DeferredRender::DeferredRender() 
{
	_wireframe = false;
	_lightsource = false;
	_windowWidth = 800;
	_windowHeight = 600;
}

gl4::DeferredRender::~DeferredRender() 
{

}


void gl4::DeferredRender::init(unsigned int windowWidth, unsigned int windowHeight) 
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;

	// init FBO with same size as window with 16 samples/pixel and 4 textures
	_standard.init(_windowWidth, _windowHeight, 16, 4);

	// init fullscreen quad
	_quad.setProportions(_windowWidth,_windowHeight);
	_quad.init();

	glm::mat4 Orthogonal = glm::ortho(0.0f,static_cast<float>(_windowWidth), 0.0f,static_cast<float>(_windowHeight), 0.1f, 100.0f);
	glm::mat4 OrthogonalView = glm::lookAt(		glm::vec3(0,0,1), // Camera pos
												glm::vec3(0,0,0), // and looks at the origin
												glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
																);
	_orthogonalProjectionMatrix = Orthogonal * OrthogonalView;

	addExtendedDeferredShaderFromFile(deferred1);
	addExtendedDeferredShaderFromFile(deferred2, 0, true);
}

void gl4::DeferredRender::addExtendedDeferredShaderFromFile(std::string name, const char *filename, bool pass2)
{
	ShaderManager::getInstance()->addShaderProgram(name, new DeferredShader(filename, pass2));
}

void gl4::DeferredRender::bindDefaultShader()
{
	bindShader(deferred1);
}

void gl4::DeferredRender::bindShader(std::string shader)
{
	ShaderManager::getInstance()->bindShader(shader);
	glUniform1f(UNIFORM_LOCATION(UNIFORM_TIME), (float)glfwGetTime());
	glUniform1i(UNIFORM_LOCATION(UNIFORM_WIREFRAME), _wireframe);
	glUniform1i(UNIFORM_LOCATION(UNIFORM_LIGHTSOURCE), _lightsource);
	glUniform1i(UNIFORM_LOCATION(UNIFORM_USETEXTURE), 0);
	glUniform2f(UNIFORM_LOCATION(UNIFORM_WINDOWSIZE), _windowWidth, _windowHeight);
}

void gl4::DeferredRender::enable(int state)
{
	switch(state) {
		case DEFERRED_WIREFRAME:
			_wireframe = true;
			break;
		case DEFERRED_LIGHTSOURCE:
			_lightsource = true;
			break;
		default:
			break;
	}
}

void gl4::DeferredRender::disable(int state)
{
	switch(state) {
		case DEFERRED_WIREFRAME:
			_wireframe = false;
			break;
		case DEFERRED_LIGHTSOURCE:
			_lightsource = false;
			break;
		default:
			break;
	}
}

void gl4::DeferredRender::useState(int uniform, bool state)
{
	//enable(state);
	switch(uniform) {
		case DEFERRED_WIREFRAME:
			glUniform1i(UNIFORM_LOCATION(UNIFORM_WIREFRAME), state);
			break;
		case DEFERRED_LIGHTSOURCE:
			break;
		case DEFERRED_TEXTURE:
			glUniform1i(UNIFORM_LOCATION(UNIFORM_USETEXTURE), state);
			break;
		default:
			break;
	}
}

void gl4::DeferredRender::render(void (*renderFunc)(void))
{
	// start stage 1
	_standard.clear();
	_standard.bind();
	bindDefaultShader();
	renderFunc();
	gl4::ShaderManager::getInstance()->unbindShader();
	_standard.unbind();
	// end stage 1

	// start stage 2
	gl4::ShaderManager::getInstance()->bindShader(deferred2);

	// orthogonal projection
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
	glm::mat4 transform = glm::mat4(1.0);
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

	// render the FBO texture
	_standard.bindTextures();
	_quad.render();

	// unbind texture and shader
	glBindTexture(GL_TEXTURE_2D,0);
	gl4::ShaderManager::getInstance()->unbindShader();
	// end stage 2
	
}
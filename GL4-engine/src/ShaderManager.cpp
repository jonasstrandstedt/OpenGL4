/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "ShaderManager.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

gl4::ShaderManager* gl4::ShaderManager::_instance = 0;

gl4::ShaderManager* gl4::ShaderManager::getInstance()
{
	//WARNING _ NOT THREAD SAFE
	if(_instance == 0)
		_instance = new ShaderManager();
	return _instance;
}

gl4::ShaderManager::ShaderManager()
{
	std::cout << "Initializing ShaderManager" << std::endl;

	// set the active shader to none
	_activeShader = 0;
	/*
	Shader *s3 = new Shader(	"GL4-engine/shaders/Deferred1_VS.glsl", 
								"GL4-engine/shaders/Deferred1_FS.glsl", 
								"GL4-engine/shaders/Deferred1_GS.glsl"
							);
*/
	Shader *s4 = new Shader(	"GL4-engine/shaders/Sphere_VS.glsl", 
								"GL4-engine/shaders/Sphere_FS.glsl", 
								"GL4-engine/shaders/Sphere_GS.glsl", 
								"GL4-engine/shaders/Sphere_TS_control.glsl", 
								"GL4-engine/shaders/Sphere_TS_eval.glsl"
							);

	Shader *s5 = new Shader("GL4-engine/shaders/Deferred2_VS.glsl", "GL4-engine/shaders/Deferred2_FS.glsl");

	//addShaderProgram("Deferred1", s3);
	addShaderProgram("Deferred1_sphere", s4);
	addShaderProgram("Deferred2", s5);
}

gl4::ShaderManager::~ShaderManager()
{
	std::map< std::string,Shader* >::iterator it;
	it = _shaders.begin();
	while (it != _shaders.end())
	{
		delete (*it).second;
	}
}

void gl4::ShaderManager::addShaderProgram(std::string name, Shader *program)
{
	std::cout << "Adding shader program [ " << name << " ]: " << std::endl;
	program->printUniforms();
	_shaders.insert(std::pair<std::string,Shader*>(name,program));
}


GLuint gl4::ShaderManager::getShaderProgram(std::string shader)
{
	std::map< std::string,Shader* >::iterator it;
	it = _shaders.find(shader);
	if (it != _shaders.end())
	{
		return (*it).second->getShaderProgram();
	} else {
		return 0;
	}
}

void gl4::ShaderManager::bindShader(std::string shader)
{
	std::map< std::string,Shader* >::iterator it;
	it = _shaders.find(shader);
	if (it != _shaders.end())
	{
		//std::cout << "Binding shader [" << shader << "]" << std::endl;
		_activeShader = (*it).second;
		glUseProgram( _activeShader->getShaderProgram());
	} else {
		std::cerr << "ShaderManager could not find [ " << shader<< " ]" << std::endl;
		_activeShader = 0;
		glUseProgram(0);
	}
}

void gl4::ShaderManager::unbindShader() 
{
	_activeShader = 0;
	glUseProgram( 0 );
}

GLint gl4::ShaderManager::getActiveShaderUniform(int uniform)
{
	if (_activeShader == 0)
	{
		return -1;
	}
	return  _activeShader->getUniformLocation(uniform);
}

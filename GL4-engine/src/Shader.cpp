/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Shader.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>

gl4::Shader::Shader()
{
	_shaderProgram = 0;

	for (int i = 0; i < 16; ++i)
	{
		_uniformLocations[i] = -1;
	}
}

gl4::Shader::Shader(const char *vertfilename, const char *fragfilename, const char *geofilename, const char *tesscontrolfilename, const char *tessevalfilename)
{
	_shaderProgram = 0;
	for (int i = 0; i < 16; ++i)
	{
		_uniformLocations[i] = -1;
	}

	init();
	attachVertexShader(vertfilename);
	attachFragmentShader(fragfilename);
	if (geofilename != 0)
	{
		attachGeometryShader(geofilename);
	}
	if (tesscontrolfilename != 0 && tessevalfilename != 0)
	{
		attachTessellationShader(tesscontrolfilename, tessevalfilename);
	}
	link();
}

gl4::Shader::~Shader()
{
	glDeleteShader(_shaderProgram);
}

void gl4::Shader::init()
{
	_shaderProgram = glCreateProgram();
}

void gl4::Shader::link()
{
	char str[4096];
	GLint shadersLinked = GL_FALSE;

	// Link the program object and print out the info log
	glLinkProgram( _shaderProgram );
	glGetProgramiv( _shaderProgram, GL_LINK_STATUS, &shadersLinked );

	if( shadersLinked == GL_FALSE )
	{
		glGetInfoLogARB( _shaderProgram, sizeof(str), NULL, str );
		std::cerr << "Program object linking error: " << str << std::endl;
	} else {
		_uniformLocations[UNIFORM_PROJECTION] = glGetUniformLocation( _shaderProgram, "Projection");
		_uniformLocations[UNIFORM_MODELTRANSFORM] = glGetUniformLocation( _shaderProgram, "ModelTransform");
		_uniformLocations[UNIFORM_TEXTURE1] = glGetUniformLocation( _shaderProgram, "texture1");
		_uniformLocations[UNIFORM_TEXTURE2] = glGetUniformLocation( _shaderProgram, "texture2");
		_uniformLocations[UNIFORM_TEXTURE3] = glGetUniformLocation( _shaderProgram, "texture3");
		_uniformLocations[UNIFORM_TESSLEVEL] = glGetUniformLocation( _shaderProgram, "TessLevel");
		_uniformLocations[UNIFORM_WIREFRAME] = glGetUniformLocation( _shaderProgram, "Wireframe");
		_uniformLocations[UNIFORM_LIGHTSOURCE] = glGetUniformLocation( _shaderProgram, "Lightsource");
		_uniformLocations[UNIFORM_USETEXTURE] = glGetUniformLocation( _shaderProgram, "UseTexture");
		_uniformLocations[UNIFORM_WINDOWSIZE] = glGetUniformLocation( _shaderProgram, "WindowSize");
		_uniformLocations[UNIFORM_TIME] = glGetUniformLocation( _shaderProgram, "time");
	}
}

GLint gl4::Shader::getUniformLocation(int uniform)
{
	return _uniformLocations[uniform];
}

void gl4::Shader::printUniforms(bool all) 
{
	_printUniform(std::cout, "UNIFORM_PROJECTION", UNIFORM_PROJECTION, all);
	_printUniform(std::cout, "UNIFORM_MODELTRANSFORM", UNIFORM_MODELTRANSFORM, all);
	_printUniform(std::cout, "UNIFORM_TIME", UNIFORM_TIME, all);
	_printUniform(std::cout, "UNIFORM_TEXTURE1", UNIFORM_TEXTURE1, all);
	_printUniform(std::cout, "UNIFORM_TEXTURE2", UNIFORM_TEXTURE2, all);
	_printUniform(std::cout, "UNIFORM_TEXTURE2", UNIFORM_TEXTURE2, all);
	_printUniform(std::cout, "UNIFORM_TESSLEVEL", UNIFORM_TESSLEVEL, all);
	_printUniform(std::cout, "UNIFORM_WIREFRAME", UNIFORM_WIREFRAME, all);
	_printUniform(std::cout, "UNIFORM_LIGHTSOURCE", UNIFORM_LIGHTSOURCE, all);
	_printUniform(std::cout, "UNIFORM_USETEXTURE", UNIFORM_USETEXTURE, all);
	_printUniform(std::cout, "UNIFORM_WINDOWSIZE", UNIFORM_WINDOWSIZE, all);
}

void gl4::Shader::_printUniform(std::ostream &out, const char *name, int uniform, bool set)
{
	if (set || _uniformLocations[uniform] > -1)
	{
		out 	<< "   " << std::setw(28) << std::left << name
				<< std::setw(3) << std::right << _uniformLocations[uniform] 
				<< std::endl;
	}
}

bool gl4::Shader::attachVertexShader(const char *filename)
{
	GLuint shader = _compileShader(GL_VERTEX_SHADER, filename, "Vertex");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}
}
bool gl4::Shader::attachFragmentShader(const char *filename)
{
	GLuint shader = _compileShader(GL_FRAGMENT_SHADER, filename, "Fragment");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}
}
bool gl4::Shader::attachGeometryShader(const char *filename)
{
	GLuint shader = _compileShader(GL_GEOMETRY_SHADER, filename, "Geometry");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}
}
bool gl4::Shader::attachTessellationShader(const char *controlFilename, const char *evalFilename)
{
	GLuint shader1 = _compileShader(GL_TESS_CONTROL_SHADER, controlFilename, "Tessellation control");
	GLuint shader2 = _compileShader(GL_TESS_EVALUATION_SHADER, evalFilename, "Tessellation evaluation");
	if (shader1 != 0 && shader2 != 0)
	{
		glAttachShader( _shaderProgram, shader1 );
		glAttachShader( _shaderProgram, shader2 );
	}
}

GLuint gl4::Shader::_compileShader(GLenum shaderType, const char *filename, const char *shaderString)
{
	GLuint shader;
	const char *shaderStrings[1];
	GLint shaderCompiled = GL_FALSE;
	char str[4096]; // For error messages from the GLSL compiler and linker


	shader = glCreateShader( shaderType );

	char *shaderAssembly = _readShaderFile( filename );
	shaderStrings[0] = shaderAssembly;
	glShaderSource( shader, 1, shaderStrings, NULL );
	glCompileShader( shader );
	free((void *)shaderAssembly);

	glGetShaderiv( shader, GL_COMPILE_STATUS, &shaderCompiled );
	if(shaderCompiled == GL_FALSE)
	{
		glGetInfoLogARB( shader, sizeof(str), NULL, str );
		std::cerr << shaderString << " shader compile error: " << str << std::endl;
		return 0;
	} else {
		return shader;
	}
}

char* gl4::Shader::_readShaderFile(const char *filename) 
{
	std::FILE *file = std::fopen(filename, "r");
	if(file == NULL)
	{
		std::cerr << "I/O error: Cannot open shader file [" << filename << "]" << std::endl;
		return 0;
	}
	fseek(file, 0, SEEK_END);
	int bytesinfile = (int)ftell(file);
	fseek(file, 0, SEEK_SET);
	char *buffer = (char*)std::malloc(bytesinfile+1);
	int bytesread = std::fread( buffer, 1, bytesinfile, file);
	buffer[bytesread] = 0; // Terminate the string with 0
	std::fclose(file);

	return buffer;
}

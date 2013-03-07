/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "DeferredShader.h"
#include "DeferredShaderSources.h"
#include <iostream>
#include <iomanip>
#include <cstring>

gl4::DeferredShader::DeferredShader(const char *filename, bool pass2)
{
	init();
	if(!pass2) 
	{
		_generateVertexShader(filename,fixed_vs_pass1);
		_generateFragmentShader(filename,fixed_fs_pass1);
		_generateGeometryShader(filename,fixed_gs_pass1);
	} else {
		_generateVertexShader(filename,fixed_vs_pass2);
		_generateFragmentShader(filename,fixed_fs_pass2);
	}
	link();
}

gl4::DeferredShader::~DeferredShader()
{
	
}

void gl4::DeferredShader::_generateVertexShader(const char *filename, const char *fixed_source)
{
	//const char * fixed_source = _readShaderFile("GL4-engine/shaders/Deferred1_VS_fixed.glsl");
	char finalsource[16384];
	strcpy(finalsource, fixed_source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"//<!-- VS start -->","//<!-- VS stop -->","void VS(){}").c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}

	GLuint shader = _compileShaderSource(GL_VERTEX_SHADER, finalsource, "Vertex");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}
}

void gl4::DeferredShader::_generateFragmentShader(const char *filename, const char *fixed_source)
{
	//const char * source = _readShaderFile("GL4-engine/shaders/Deferred1_FS_fixed.glsl");
	char finalsource[16384];
	strcpy(finalsource, fixed_source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"//<!-- FS start -->","//<!-- FS stop -->","void FS()").c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}

	GLuint shader = _compileShaderSource(GL_FRAGMENT_SHADER, finalsource, "Fragment");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}

}

void gl4::DeferredShader::_generateGeometryShader(const char *filename, const char *fixed_source)
{
	//const char * source = _readShaderFile("GL4-engine/shaders/Deferred1_GS_fixed.glsl");
	char finalsource[16384];
	strcpy(finalsource, fixed_source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"//<!-- GS start -->","//<!-- GS stop -->","void GS(int i){}").c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}

	GLuint shader = _compileShaderSource(GL_GEOMETRY_SHADER, finalsource, "Geometry");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}
}

GLuint gl4::DeferredShader::_compileShaderSource(GLenum shaderType, const char *source, const char *shaderString)
{
	GLuint shader;
	const char *shaderStrings[1];
	GLint shaderCompiled = GL_FALSE;
	char str[4096]; // For error messages from the GLSL compiler and linker


	shader = glCreateShader( shaderType );

	shaderStrings[0] = source;
	glShaderSource( shader, 1, shaderStrings, NULL );
	glCompileShader( shader );

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

std::string gl4::DeferredShader::_getPartOfString(std::string s, std::string s1, std::string s2, std::string nothing) 
{
	size_t found1 = s.find(s1);

	if (found1 ==s.npos)
		return nothing;

	size_t found2 = s.find(s2,found1+1);
	if (found2==s.npos)
		return nothing;

	return s.substr(found1,found2-found1);
}
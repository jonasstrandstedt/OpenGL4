/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/glfw.h>

#include <ostream>

enum {
	UNIFORM_PROJECTION = 0,
	UNIFORM_MODELTRANSFORM,
	UNIFORM_TEXTURE1,
	UNIFORM_TEXTURE2,
	UNIFORM_TEXTURE3,
	UNIFORM_TESSLEVEL,
	UNIFORM_WIREFRAME,
	UNIFORM_LIGHTSOURCE,
	UNIFORM_USETEXTURE,
	UNIFORM_WINDOWSIZE
};


namespace gl4
{
	class Shader
	{
	public:


		//initializers
		Shader();
		Shader(const char *vertfilename, const char *fragfilename, const char *geofilename = 0, const char *tesscontrolfilename = 0, const char *tessevalfilename = 0);
		~Shader();

		void init();
		void link();

		bool attachVertexShader(const char *filename);
		bool attachFragmentShader(const char *filename);
		bool attachGeometryShader(const char *filename);
		bool attachTessellationShader(const char *controlFilename, const char *evalFilename);

		GLuint getShaderProgram() { return _shaderProgram; };

		GLint getUniformLocation(int uniform);
		void printUniforms(bool all = false);


	private:

		void _printUniform(std::ostream &out, const char *name, int uniform, bool set = false);

		GLuint _shaderProgram;

		GLint _uniformLocations[16];

		GLuint _compileShader(GLenum shaderType, const char *filename, const char *shaderString);
		char* _readShaderFile(const char *filename);
	};
}

#endif
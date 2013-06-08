/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef DEFERREDSHADER_H
#define DEFERREDSHADER_H

#include "Shader.h"


namespace gl4
{
	class DeferredShader: public Shader
	{
	public:

		//initializers
		DeferredShader(const char *filename = 0, bool pass2 = false);
		~DeferredShader();

	private:
		GLuint _compileShaderSource(GLenum shaderType, const char *source, const char *shaderString);
		const char* _getPartOfString(std::string s, std::string s1, std::string s2, const char *nothing = 0);

		void _generateVertexShader(const char *filename, const char *fixed_source);
		void _generateFragmentShader(const char *filename, const char *fixed_source);
		void _generateGeometryShader(const char *filename, const char *fixed_source);
	};
}

#endif
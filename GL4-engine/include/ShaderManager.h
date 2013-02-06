/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Engine.h"
#include "Shader.h"
#include <string>
#include <map>
#include <vector>

#define UNIFORM_LOCATION(i) gl4::ShaderManager::getInstance()->getActiveShaderUniform(i)

namespace gl4
{
	class ShaderManager 
	{
	public:
		//initializers
		
		~ShaderManager();
		static ShaderManager* getInstance();

		void addShaderProgram(std::string name, Shader *program);
		GLuint getShaderProgram(unsigned int i);
		void bindShader(unsigned int i);
		GLuint getShaderProgram(std::string shader);
		void bindShader(std::string shader);
		void unbindShader();

		GLint getActiveShaderUniform(int uniform);

	private:
		
		// singleton constructor
		ShaderManager();
		static ShaderManager* _instance;


		Shader *_activeShader;
		std::map< std::string, Shader* > _shaders;
	};
}

#endif
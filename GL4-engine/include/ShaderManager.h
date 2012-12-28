#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Engine.h"
#include <string>
#include <map>

namespace gl4
{
	class ShaderManager 
	{
	public:
		//initializers
		
		~ShaderManager();
		static ShaderManager* getInstance();

		GLuint getShaderProgram(unsigned int i);
		void bindShader(unsigned int i);
		GLuint getShaderProgram(std::string shader);
		void bindShader(std::string shader);
		void unbindShader();

	private:
		ShaderManager();
		static ShaderManager* _instance;

		std::map< std::string, GLuint > _shaders;

		char* _readShaderFile(const char *filename);
		GLuint _createShader(const char *vertfilename, const char *fragfilename);
	};
}

#endif
/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Engine.h"
#include <string>
#include <map>

namespace gl4
{
	class TextureManager 
	{
	public:
		~TextureManager();
		static TextureManager* getInstance();

		void loadTexture(std::string name, const char *filename);
		GLuint getTexture(std::string name);
		void bindTexture(std::string name, unsigned int position = 0);

	private:
		TextureManager();
		static TextureManager* _instance;

		// data
		GLint _GL_MAX_TEXTURE_SIZE;
		GLint _GL_MAX_TEXTURE_BUFFER_SIZE;

		std::map< std::string, GLuint > _textures;

		// private functions
		bool _compare_extension(const char *filename, const char *extension);
		GLuint _loadTextureFromPNG(const char * file_name, int * width = 0, int * height = 0);
		GLuint _loadTextureFromTGA(const char *filename);
	};
}

#endif
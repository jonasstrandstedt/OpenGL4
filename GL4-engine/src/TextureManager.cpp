/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "TextureManager.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

gl4::TextureManager* gl4::TextureManager::_instance = 0;

gl4::TextureManager* gl4::TextureManager::getInstance()
{
	//WARNING _ NOT THREAD SAFE
	if(_instance == 0)
		_instance = new TextureManager();
	return _instance;
}

gl4::TextureManager::TextureManager()
{
	std::cout << "Initializing TextureManager" << std::endl;



}

gl4::TextureManager::~TextureManager()
{

}

void gl4::TextureManager::loadTexture(std::string name, const char *filename)
{
	std::cout << "Loading texture:" << std::endl;

	GLuint textureID = 0;
	std::map< std::string,GLuint >::iterator it;
	it = _textures.find(name);
	
	if (it == _textures.end())
	{
		if (_compare_extension(filename, ".tga"))
		{
			textureID = _loadTextureFromTGA(filename);
		} else {
			std::cerr << "   Unsupported file format [" << filename<< "]" << std::endl;
		}
	} else {
		std::cerr << "   Texture name already occupied! [" << name << "]" << std::endl;
	}

	if (textureID == 0)
	{
		std::cerr << "   Loading texture failed!" << std::endl;
	} else {
		_textures.insert(std::pair<std::string,GLuint>(name,textureID));
		std::cout << "   Name: " << name << std::endl;
		std::cout << "   TextureID: " << textureID << std::endl;
		std::cout << "   Loading texture success!" << std::endl;
	}
	
}

GLuint gl4::TextureManager::getTexture(std::string name)
{
	std::map< std::string,GLuint >::iterator it;
	it = _textures.find(name);
	if (it != _textures.end())
	{
		return (*it).second;
	} else {
		return 0;
	}
}

/**
*	 http://stackoverflow.com/questions/51949/how-to-get-file-extension-from-string-in-c
*/
bool gl4::TextureManager::_compare_extension(const char *filename, const char *extension)
{
    /* Sanity checks */

    if(filename == NULL || extension == NULL)
        return false;

    if(strlen(filename) == 0 || strlen(extension) == 0)
        return false;

    if(strchr(filename, '.') == NULL || strchr(extension, '.') == NULL)
        return false;

    /* Iterate backwards through respective strings and compare each char one at a time */

    for(int i = 0; i < strlen(filename); i++)
    {
        if(tolower(filename[strlen(filename) - i - 1]) == tolower(extension[strlen(extension) - i - 1]))
        {
            if(i == strlen(extension) - 1)
                return true;
        } else
            break;
    }

    return false;
}

GLuint gl4::TextureManager::_loadTextureFromTGA(const char *filename) {
	GLuint textureID = 0;
	GLFWimage img;

	if(!glfwReadImage(filename, &img, GLFW_NO_RESCALE_BIT))
		std::cerr << "   Failed to load texture from TGA file." << std::endl;

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, textureID );

	glfwLoadTextureImage2D( &img, 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glfwFreeImage(&img); // Clean up the malloc()'ed data pointer
	return textureID;
}
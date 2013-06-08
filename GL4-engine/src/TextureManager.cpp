/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "TextureManager.h"

#ifdef LIBPNG
#include "png.h"
#endif

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

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_GL_MAX_TEXTURE_SIZE);
	glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &_GL_MAX_TEXTURE_BUFFER_SIZE);

	std::cout << "   GL_MAX_TEXTURE_SIZE = " << _GL_MAX_TEXTURE_SIZE << std::endl;
	std::cout << "   GL_MAX_TEXTURE_BUFFER_SIZE = " << _GL_MAX_TEXTURE_BUFFER_SIZE << std::endl;



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
		} else if(_compare_extension(filename, ".png")) {
			textureID = _loadTextureFromPNG(filename);
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

void gl4::TextureManager::bindTexture(std::string name, unsigned int position)
{
	std::map< std::string,GLuint >::iterator it;
	it = _textures.find(name);
	if (it != _textures.end())
	{

		glActiveTexture(GL_TEXTURE0 + position);
		glBindTexture(GL_TEXTURE_2D, (*it).second);
		glUniform1i(UNIFORM_LOCATION(UNIFORM_TEXTURE1 + position), position);
	}
}

/**
* From:	http://stackoverflow.com/a/4777391
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

/**
* From: http://stackoverflow.com/a/11297197
*/
GLuint gl4::TextureManager::_loadTextureFromPNG(const char * file_name, int * width, int * height)
{
#ifdef LIBPNG
	png_byte header[8];

	FILE *fp = fopen(file_name, "rb");
	if (fp == 0)
	{
		perror(file_name);
		return 0;
	}

    // read the header
	fread(header, 1, 8, fp);

	if (png_sig_cmp(header, 0, 8))
	{
		fprintf(stderr, "error: %s is not a PNG.\n", file_name);
		fclose(fp);
		return 0;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fprintf(stderr, "error: png_create_read_struct returned 0.\n");
		fclose(fp);
		return 0;
	}

    // create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}

    // create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return 0;
	}

    // the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "error from libpng\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

    // init png reading
	png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;

    // get info about png
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
		NULL, NULL, NULL);

	if (width != 0){ *width = temp_width; }
	if (height != 0){ *height = temp_height; }

    // Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
	rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
	png_byte * image_data;
	image_data = (png_byte*)std::malloc(rowbytes * temp_height * sizeof(png_byte)+15);
	if (image_data == NULL)
	{
		fprintf(stderr, "error: could not allocate memory for PNG image data\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

    // row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep * row_pointers = (png_bytep*)std::malloc(temp_height * sizeof(png_bytep));
	if (row_pointers == NULL)
	{
		fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		fclose(fp);
		return 0;
	}

    // set the individual row_pointers to point at the correct offsets of image_data
	int i;
	for (i = 0; i < temp_height; i++)
	{
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	}

    // read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_width, temp_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(image_data);
	free(row_pointers);
	fclose(fp);
	return texture;
#else 
	return 0;
#endif
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
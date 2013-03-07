/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "FBO.h"
#include "ShaderManager.h"

#include <iostream>
#include <cstdlib>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

gl4::FBO::FBO() {
	_fboId = -1;
	//_fboTextureId = -1;
	_fboId_multisampled = -1;
	_w = 0;
	_h = 0;
	_multisampled= false;
	_textures = 0;
}

gl4::FBO::~FBO() {
	delete _fboTextureId;
}

void gl4::FBO::init(GLuint width, GLuint height, GLuint samples, GLuint textures) {
	_textures = textures;

	_fboTextureId = new GLuint[textures];
	GLuint *_rboId = new GLuint[textures];
	GLuint _dboId = -1;
	GLuint *_rboId_multisampled = new GLuint[textures];
	GLuint _dboId_multisampled = -1;

	_w = width;
	_h = height;

	GLenum errorID;
	GLenum status;

	std::cout << "FBO: Initialize with size " << width << ", " << height << std::endl;
	std::cout << "   Textures = " << textures << std::endl;
	GLint MaxSamples;
	glGetIntegerv(GL_MAX_SAMPLES, &MaxSamples);
	std::cout << "   MaxSamples = " << MaxSamples << std::endl;
	if ( samples > 0 && samples <= MaxSamples)
	{
		_multisampled = true;
		std::cout << "   Samples = " << samples << std::endl;
	}


	glGenFramebuffers(1, &_fboId);
	glGenTextures(textures, _fboTextureId);
	glGenRenderbuffers(textures, _rboId);
	glGenRenderbuffers(1, &_dboId);

	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);

	_buffers = new GLenum[_textures];
	for (int i = 0; i < textures; ++i)
	{
		_buffers[i] = GL_COLOR_ATTACHMENT0+i;
		glBindRenderbuffer(GL_RENDERBUFFER, _rboId[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_RENDERBUFFER, _rboId[i]);
	}
	

	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	glBindRenderbuffer(GL_RENDERBUFFER, _dboId);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, _dboId);

	for (int i = 0; i < textures; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, _fboTextureId[i]);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, _fboTextureId[i], 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	
	
	// check FBO status
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "   " << "Error creating FBO: " << status << std::endl;
	else
		std::cout << "   " << "FBO created successfully [" << _fboId << "]" <<  std::endl;

	// switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	if (_multisampled)
	{
		glGenFramebuffers(1, &_fboId_multisampled);
		glGenRenderbuffers(textures, _rboId_multisampled);
		glGenRenderbuffers(1, &_dboId_multisampled);

		glBindFramebuffer(GL_FRAMEBUFFER, _fboId_multisampled);

		for (int i = 0; i < textures; ++i)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, _rboId_multisampled[i]);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_RENDERBUFFER, _rboId_multisampled[i]);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, _fboId_multisampled);
		glBindRenderbuffer(GL_RENDERBUFFER, _dboId_multisampled);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT32, width, height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, _dboId_multisampled);

		// check FBO status
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "   " << "Error creating FBO: " << status << std::endl;
		else
			std::cout << "   " << "Multisampled FBO created successfully [" << _fboId_multisampled << "]" <<  std::endl;

		// switch back to window-system-provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void gl4::FBO::bind() {
	glBindTexture(GL_TEXTURE_2D, 0);
	//glEnable(GL_MULTISAMPLE);
	if (_multisampled)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fboId_multisampled);
	} else {

		glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	}

	glDrawBuffers(_textures, _buffers);

	GLuint errorID = glGetError();
	if(errorID != GL_NO_ERROR)
	{
		std::cerr 	<< " OpenGL error: " << glewGetErrorString(errorID) << std::endl
					<< "Attempting to proceed anyway. Expect rendering errors or a crash." << std::endl;
	}
}

void gl4::FBO::unbind() {
	if (_multisampled)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fboId_multisampled); // source FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fboId); // dest FBO

		// for-loop to copy all the buffers, one by one
		for (int i = 0; i < _textures; ++i)
		{
			glReadBuffer( GL_COLOR_ATTACHMENT0+i );
			glDrawBuffer( GL_COLOR_ATTACHMENT0+i );
			glBlitFramebuffer(
								0, 0, _w, _h,
								0, 0, _w, _h,
								GL_COLOR_BUFFER_BIT, GL_NEAREST
			);
		}
		
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void gl4::FBO::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (_multisampled)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fboId_multisampled);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

GLuint gl4::FBO::getTexture(GLuint i)
{
	return _fboTextureId[i];
}

void gl4::FBO::bindTextures()
{
	for (int i = 0; i < _textures; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _fboTextureId[i]);
		glUniform1i(UNIFORM_LOCATION(UNIFORM_TEXTURE1 + i), i);
	}
}

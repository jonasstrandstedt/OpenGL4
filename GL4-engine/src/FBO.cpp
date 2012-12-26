#include "FBO.h"

#include <iostream>
#include <cstdlib>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

gl4::FBO::FBO() {
	_fboId = -1;
	_fboTextureId = -1;
	_fboId_multisampled = -1;
	_w = 0;
	_h = 0;
	_multisampled= false;
}

gl4::FBO::~FBO() {
	
}

void gl4::FBO::init(GLuint width, GLuint height, GLuint samples) {


	GLuint _rboId = -1;
	GLuint _dboId = -1;
	GLuint _rboId_multisampled = -1;
	GLuint _dboId_multisampled = -1;

	_w = width;
	_h = height;

	GLenum errorID;
	GLenum status;

	std::cout << "FBO: Initialize with size " << width << ", " << height << std::endl;
	GLint MaxSamples;
	glGetIntegerv(GL_MAX_SAMPLES, &MaxSamples);
	std::cout << "   MaxSamples = " << MaxSamples << std::endl;
	if ( samples > 0 && samples <= MaxSamples)
	{
		_multisampled = true;
		std::cout << "   Samples = " << samples << std::endl;
	}


	glGenFramebuffers(1, &_fboId);
	glGenTextures(1, &_fboTextureId);
	glGenRenderbuffers(1, &_rboId);
	glGenRenderbuffers(1, &_dboId);

	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);

	glBindRenderbuffer(GL_RENDERBUFFER, _rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _rboId);

	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	glBindRenderbuffer(GL_RENDERBUFFER, _dboId);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, _dboId);


	glBindTexture(GL_TEXTURE_2D, _fboTextureId);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTextureId, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
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
		glGenRenderbuffers(1, &_rboId_multisampled);
		glGenRenderbuffers(1, &_dboId_multisampled);

		glBindFramebuffer(GL_FRAMEBUFFER, _fboId_multisampled);

		glBindRenderbuffer(GL_RENDERBUFFER, _rboId_multisampled);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _rboId_multisampled);

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
}

void gl4::FBO::unbind() {
	if (_multisampled)
	{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fboId_multisampled); // source
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fboId); // dest
	glBlitFramebuffer(
		0, 0, _w, _h,
		0, 0, _w, _h,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
}


glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_MULTISAMPLE);
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

GLuint gl4::FBO::getTexture()
{
	return _fboTextureId;
}

void gl4::FBO::check() {
	/*
	std::cout << "Checking.." << std::endl;
	GLint maxRectTextureSize;
	GLint myMaxTextureUnits;
	GLint myMaxTextureSize;
	const GLubyte * strVersion;
	const GLubyte * strExt;
	float myGLVersion;
	GLboolean isVAO, isTexLOD, isColorTable, isFence, isShade,
	          isTextureRectangle;
	strVersion = glGetString (GL_VERSION); // 1
	sscanf((char *)strVersion, "%f", &myGLVersion);
	strExt = glGetString (GL_EXTENSIONS); // 2
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &myMaxTextureUnits); // 3
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &myMaxTextureSize); // 4
	isVAO =
	    gluCheckExtension ((const GLubyte*)"GL_APPLE_vertex_array_object",strExt); // 5
	isFence = gluCheckExtension ((const GLubyte*)"GL_APPLE_fence", strExt); // 6
	isShade =
	     gluCheckExtension ((const GLubyte*)"GL_ARB_shading_language_100", strExt); // 7
	isColorTable =
	     gluCheckExtension ((const GLubyte*)"GL_SGI_color_table", strExt) ||
	             gluCheckExtension ((const GLubyte*)"GL_ARB_imaging", strExt); // 8
	isTexLOD =
	     gluCheckExtension ((const GLubyte*)"GL_SGIS_texture_lod", strExt) ||
	                                  (myGLVersion >= 1.2); // 9
	isTextureRectangle = gluCheckExtension ((const GLubyte*)
	                                 "GL_EXT_texture_rectangle", strExt);
	if (isTextureRectangle)
	      glGetIntegerv (GL_MAX_RECTANGLE_TEXTURE_SIZE, &maxRectTextureSize);
	else
	     maxRectTextureSize = 0;
	
	std::cout << strVersion <<std:: endl << strExt << std::endl << std::endl;
	if(isVAO && isFence && isColorTable && isTexLOD && isTextureRectangle) {
		std::cout << "I have all the extensions!" << std::endl;
	} else {
		std::cout << std::boolalpha;
		std::cout << "isVAO = " << (bool)isVAO << std::endl;
		std::cout << "isFence = " << (bool)isFence << std::endl;
		std::cout << "isColorTable = " << (bool)isColorTable << std::endl;
		std::cout << "isTexLOD = " << (bool)isTexLOD << std::endl;
		std::cout << "isTextureRectangle = " << (bool)isTextureRectangle << std::endl;
		
	}		
	std::cout << "maxRectTextureSize = " << maxRectTextureSize << std::endl;
	*/
}

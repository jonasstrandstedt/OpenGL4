/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef FBO_H
#define FBO_H

#include <GL/glew.h>
#include <GL/glfw.h>


namespace gl4
{
class FBO
{
	public:
		FBO();
		~FBO();
		
		void init(GLuint width, GLuint height, GLuint samples = 0, GLuint textures = 1);
		void bind();
		void unbind();
		void clear();

		GLuint getTexture(GLuint i = 0);
		void bindTextures();

	private:
		GLuint *_fboTextureId;
		GLuint _fboId_multisampled;
		GLuint _fboId;
		GLuint _textures;
		GLenum *_buffers;

		bool _multisampled;

		int _w;
		int _h;
};
}

#endif
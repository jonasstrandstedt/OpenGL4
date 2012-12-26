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
		
		void init(GLuint width, GLuint height, GLuint samples = 0);
		void bind();
		void unbind();
		void clear();

		GLuint getTexture();
		
		static void check();
	private:
		GLuint _fboTextureId;
		GLuint _fboId_multisampled;
		GLuint _fboId;

		bool _multisampled;

		int _w;
		int _h;
};
}

#endif
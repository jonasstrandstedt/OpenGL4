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
		void bindTextures(GLuint uniformStart);

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
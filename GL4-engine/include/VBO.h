#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <GL/glfw.h>


#define BUFFER_OFFSET(i) ((char *)NULL + (i))

typedef struct
{
	GLfloat location[3];
	GLfloat tex[2];
	GLfloat normal[3];
	GLfloat color[4];
	GLfloat attribute[3];
	GLfloat float_attribute;
	//GLubyte padding[4]; // Pads the struct out to 64 bytes for performance increase
} Vertex;

namespace gl4
{
	class VBO 
	{
	public:
		//initializers
		VBO();
		~VBO();

		// init VBO
		void init(float w = 1.0f, float h = 1.0f);

		// render
		void render();
	private:

		GLuint _vaoID;
		GLuint _vBufferID;
		GLuint _iBufferID;

	protected:
		// arrays with all triangles and indices
		unsigned int _isize;
		unsigned int _vsize;
		Vertex *_varray;
		int *_iarray;
	};
}

#endif
#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "VBO.h"
#include <vector>
#include <glm/glm.hpp>

namespace gl4
{
	class FontManager
	{
	public:
		static FontManager * getInstance();

		void render(float width=1.0f, float height=1.0f);
		void addText(float x, float y, const char * text, float fontSize=32.0f, glm::vec4 color=glm::vec4(1.0));
		void clearText();

		void printText(float x, float y, const char * text, float fontSize=32.0f, glm::vec4 color=glm::vec4(1.0));
	private:
		static FontManager * instance;

		typedef struct
		{
			int x,y,width,height;
			float x_offset,y_offset,x_advance;
			float s0, t0, s1, t1;
			unsigned char index;
		}Character;

		class FontBuffer;

		FontManager();
		~FontManager();

		void loadFontData(const char * filename, int width, int height);

		FontBuffer * buffer;
		std::vector<Character> charmap;

		int fontmapWidth, fontmapHeight;
		float characterSize;
	};

	class FontManager::FontBuffer
	{
	public:
		FontBuffer();
		~FontBuffer();

		void init();

		void push_back(Vertex * vData, int vCount, int * iData, int iCount);
		void render();
		void clear();
	private:
		std::vector<Vertex> vertices;
		std::vector<int> indices;

		GLuint _vaoID;
		GLuint _vBufferID;
		GLuint _iBufferID;
	};
}

#endif


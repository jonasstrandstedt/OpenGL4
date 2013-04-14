#include "FontManager.h"
#include "TextureManager.h"

#include <fstream>
#include <sstream>

gl4::FontManager * gl4::FontManager::instance = NULL;

inline void setVertex(	Vertex * v,
						float x, float y, float z,
						float nx, float ny, float nz,
						float s, float t,
						const glm::vec4 &color,
						float a0=0.0f, float a1=0.0f, float a2=0.0f,
						float fa=0.0f)
{
    v->location[0] = x;
    v->location[1] = y;
    v->location[2] = z;

	v->normal[0] = nx;
	v->normal[1] = ny;
	v->normal[2] = nz;

	v->tex[0] = s;
	v->tex[1] = t;

	v->color[0] = color.x;
	v->color[1] = color.y;
	v->color[2] = color.z;
	v->color[3] = color.w;

	v->attribute[0] = a0;
	v->attribute[1] = a1;
	v->attribute[2] = a2;

	v->float_attribute = fa;
}

gl4::FontManager * gl4::FontManager::getInstance()
{
	if(!FontManager::instance)
	{
		FontManager::instance = new FontManager();
	}
	return FontManager::instance;
}

void gl4::FontManager::render(float width, float height)
{

	width = 1024.0f;
	height = 768.0f;
	glViewport(0, 0, width, height);

	glm::mat4 Orthogonal = glm::ortho(0.0f, width, 0.0f, height, 0.1f, 100.0f);
	glm::mat4 OrthogonalView = glm::lookAt(		glm::vec3(0,0,1), // Camera pos
												glm::vec3(0,0,0), // and looks at the origin
												glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
																);
	glm::mat4 _orthogonalProjectionMatrix = Orthogonal * OrthogonalView;

	float size = 1.0;
	float pixelScale = 1.0;
	float scalex = 1.0;
	float scaley = 1.0;

	gl4::ShaderManager::getInstance()->bindShader("text");
	gl4::TextureManager::getInstance()->bindTexture("fontmap", 0);
	GLuint program = gl4::ShaderManager::getInstance()->getShaderProgram("text");
	int colorLoc = glGetUniformLocation(program, "text");

	glm::mat4 transform = glm::mat4();

	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_PROJECTION), 1, GL_FALSE, &_orthogonalProjectionMatrix[0][0]);
	glUniformMatrix4fv(UNIFORM_LOCATION(UNIFORM_MODELTRANSFORM), 1, GL_FALSE, &transform[0][0]);

	glEnable(GL_BLEND);

	GLboolean params = false;
	glGetBooleanv(GL_DEPTH_TEST, &params);
	if (params)
	{
		glDisable (GL_DEPTH_TEST);
	}

	// Lite oklart vilken blend som krävs
    //glBlendEquation(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	buffer->render();

	glDisable(GL_BLEND);
	//glEnable (GL_DEPTH_TEST);

	if (params)
	{
		glEnable (GL_DEPTH_TEST);
	}

}

void gl4::FontManager::printText(float x, float y, const char *text, float fontSize, glm::vec4 color)
{
	addText(x,y,text,fontSize,color);
	render();
	clearText();
}

void gl4::FontManager::addText(float x, float y, const char * text, float fontSize, glm::vec4 color)
{
	float scale = fontSize / characterSize;

	float xpos = x;
	float ypos = y;

    for(size_t i=0; i<strlen(text); ++i )
    {
    	unsigned char index = (unsigned char)text[i];
    	Character * c = &charmap[index];

    	if(text[i] == '\n')
    	{
    		ypos -= fontSize;
    		xpos = x;
    	}
    	else
    	{
    		float x0  = xpos + c->x_offset * scale;
	        float y0  = ypos - c->y_offset * scale;
	        float x1  = x0 + c->width * scale;
	        float y1  = y0 - c->height * scale;

	        float s0 = c->s0;
	        float t0 = c->t0;
	        float s1 = c->s1;
	        float t1 = c->t1;

	        int ind[6] = {0,1,2, 0,2,3};
	        Vertex vert[4];

	        setVertex(&vert[0], x0,y0,0.0,	0.0,0.0,1.0,	s0,t0,	color);
	        setVertex(&vert[1], x0,y1,0.0,	0.0,0.0,1.0,	s0,t1,	color);
	        setVertex(&vert[2], x1,y1,0.0,	0.0,0.0,1.0,	s1,t1,	color);
	        setVertex(&vert[3], x1,y0,0.0,	0.0,0.0,1.0,	s1,t0,	color);

        //printf("x0: %1.2f, y0: %1.2f, x1: %1.2f, y1: %1.2f \n",x0,y0,x1,y1);

	        buffer->push_back(vert, 4, ind, 6 );
	        xpos += c->x_advance * scale;
    	}
    }
}

void gl4::FontManager::clearText()
{
	buffer->clear();
}

gl4::FontManager::FontManager()
{
	const char * fontfile = "data/fonts/arial.fnt";
	const char * fontimage = "data/fonts/arial_dist.png";
	fontmapWidth = 512;
	fontmapHeight = 512;
	characterSize = 64.0f;

	gl4::TextureManager::getInstance()->loadTexture("fontmap", fontimage);
	loadFontData(fontfile, 512, 512);

	buffer = new FontBuffer();
}

gl4::FontManager::~FontManager()
{
	delete buffer;
}

void gl4::FontManager::loadFontData(const char * filename, int width, int height)
{
	printf("loadFontData \n");
    charmap.clear();
    charmap.reserve(256);

    Character c;
    c.x = c.y = c.width = c.height = 0;
    c.x_offset=c.y_offset=c.x_advance = 0.0f;

    for(int i=0; i<256; ++i)
    {
        c.index = i;
        charmap.push_back(c);
    }

    std::string param;
    std::string junk;
    std::string line;

    std::ifstream file;
    file.open(filename, std::ifstream::in);

    if ( file.fail() )
        file.close();

    getline(file,line);
    getline(file,line);

    getline(file,line);

    int pos;
    int index;

    while ( !file.eof() )
    {
        std::istringstream ss(line, std::istringstream::in);

        ss >> param;
        if(param == "char")
        {
        	//printf("found char! \n");

            ss >> param; pos = param.find('=');
            index = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.x = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.y = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.width = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.height = atoi(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.x_offset = atof(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.y_offset = atof(param.substr(pos+1,param.size()-1).c_str());

            ss >> param; pos = param.find('=');
            c.x_advance = atof(param.substr(pos+1,param.size()-1).c_str());

        	c.s0 = (float)c.x / (float)width;
        	c.t0 = 1.0 - (float)c.y / (float)height;
        	c.s1 = (float)(c.x+c.width) / (float)width;
        	c.t1 = 1.0 - (float)(c.y+c.height) / (float)height;

        	//printf("%i %i %i %i %i %f %f %f \n", index, c.x, c.y, c.width, c.height, c.x_offset, c.y_offset, c.x_advance);

            charmap[index] = c;
        }

        std::getline(file,line);
    }

    file.close();
}

gl4::FontManager::FontBuffer::FontBuffer()
{
	vertices.reserve(1024);
	indices.reserve(1024);

	init();
}

gl4::FontManager::FontBuffer::~FontBuffer()
{

}

void gl4::FontManager::FontBuffer::push_back(Vertex * vData, int vCount, int * iData, int iCount)
{
	int offset = (int)vertices.size();

	for(int i=0; i<vCount; ++i)
		vertices.push_back(vData[i]);

	for(int i=0; i<iCount; ++i)
		indices.push_back(offset+iData[i]);
}

void gl4::FontManager::FontBuffer::render()
{
	glBindVertexArray(_vaoID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iBufferID);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(int) * indices.size(), indices.data());

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	glBindVertexArray(0);
}

void gl4::FontManager::FontBuffer::clear()
{
	vertices.clear();
	indices.clear();
}

void gl4::FontManager::FontBuffer::init()
{
	GLuint errorID = glGetError();
	glGenVertexArrays(1, &_vaoID);

	// First VAO setup
	glBindVertexArray(_vaoID);

	glGenBuffers(1, &_vBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, _vBufferID);
	glBufferData(GL_ARRAY_BUFFER, 1024*sizeof(Vertex), 0, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(12));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(20));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(32));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(48));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(60));
	glEnableVertexAttribArray(5);

	glGenBuffers(1, &_iBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1024*sizeof(int), 0, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);

	if(_vBufferID == 0)
	{
		std::cerr << "Vertex buffer not initialized" << std::endl;
	}
	if(_iBufferID == 0) 
	{
		std::cerr << "Index buffer not initialized" << std::endl;
	}

	errorID = glGetError();
	if(errorID != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error: " << glewGetErrorString(errorID) << std::endl;
		std::cerr << "Attempting to proceed anyway. Expect rendering errors or a crash." << std::endl;
	}
}
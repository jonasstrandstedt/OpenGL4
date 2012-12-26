#include "ShaderManager.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

gl4::ShaderManager* gl4::ShaderManager::_instance = 0;


gl4::ShaderManager* gl4::ShaderManager::getInstance()
{
	//WARNING _ NOT THREAD SAFE
	if(_instance == 0)
		_instance = new ShaderManager();
	return _instance;
}

gl4::ShaderManager::ShaderManager()
{
	std::cout << "Initializing ShaderManager" << std::endl;

	_shaders.insert( std::pair<std::string,GLuint>("Passthrough", _createShader("GL4-engine/shaders/Passthrough.vert","GL4-engine/shaders/Passthrough.frag")));

	_shaders.insert( std::pair<std::string,GLuint>("Textured", _createShader("GL4-engine/shaders/Passthrough.vert","GL4-engine/shaders/Textured.frag")));

	//_shaders.push_back(_createShader("GL4-engine/shaders/Passthrough.vert","GL4-engine/shaders/Passthrough.frag"));
	//_shaders.push_back(_createShader("GL4-engine/shaders/Passthrough.vert","GL4-engine/shaders/Textured.frag"));
}

gl4::ShaderManager::~ShaderManager()
{

}


GLuint gl4::ShaderManager::getShaderProgram(std::string shader)
{
	std::map< std::string,GLuint >::iterator it;
	it = _shaders.find(shader);
	if (it != _shaders.end())
	{
		return (*it).second;
	} else {
		return 0;
	}
}
void gl4::ShaderManager::bindShader(std::string shader)
{
	std::map< std::string,GLuint >::iterator it;
	it = _shaders.find(shader);
	if (it != _shaders.end())
	{
		glUseProgram( (*it).second);
	} else {
		glUseProgram(0);
	}
}
GLuint gl4::ShaderManager::getShaderProgram(unsigned int i)
{
	if (i < _shaders.size())
	{
		std::map< std::string,GLuint >::iterator it = _shaders.begin();
		for (int j = 0; j < i; ++j)
		{
			it++;
		}
 	 	return (*it).second;
	}
 	return 0;
}

void gl4::ShaderManager::bindShader(unsigned int i)
{
	if (i < _shaders.size())
	{
		std::map< std::string,GLuint >::iterator it = _shaders.begin();
		for (int j = 0; j < i; ++j)
		{
			it++;
		}
 	 	glUseProgram( (*it).second );
	} else {
		glUseProgram( 0 );
	}
 	
}
void gl4::ShaderManager::unbindShader() 
{

	glUseProgram( 0 );
}

char* gl4::ShaderManager::_readShaderFile(const char *filename) 
{
	std::FILE *file = std::fopen(filename, "r");
	if(file == NULL)
	{
		std::cerr << "I/O error: Cannot open shader file [" << filename << "]" << std::endl;
		return 0;
	}
	fseek(file, 0, SEEK_END);
	int bytesinfile = (int)ftell(file);
	fseek(file, 0, SEEK_SET);
	char *buffer = (char*)std::malloc(bytesinfile+1);
	int bytesread = std::fread( buffer, 1, bytesinfile, file);
	buffer[bytesread] = 0; // Terminate the string with 0
	std::fclose(file);

	return buffer;
}

GLuint gl4::ShaderManager::_createShader(const char *vertfilename, const char *fragfilename) 
{
	GLuint programObj;
	GLuint fragmentShader;
	GLuint vertexShader;
	const char *vertexShaderStrings[1];
	GLint vertexCompiled = GL_FALSE;
	const char *fragmentShaderStrings[1];
	GLint fragmentCompiled = GL_FALSE;
	GLint shadersLinked = GL_FALSE;
	char str[4096]; // For error messages from the GLSL compiler and linker

  // Create the vertex and fragment shaders
	vertexShader = glCreateShader( GL_VERTEX_SHADER );

	char *vertexShaderAssembly = _readShaderFile( vertfilename );
	vertexShaderStrings[0] = vertexShaderAssembly;
	glShaderSource( vertexShader, 1, vertexShaderStrings, NULL );
	glCompileShader( vertexShader );
	free((void *)vertexShaderAssembly);

	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vertexCompiled );
	if(vertexCompiled == GL_FALSE)
	{
		glGetInfoLogARB( vertexShader, sizeof(str), NULL, str );
		std::cerr << "Vertex shader compile error: " << str << std::endl;
	}

	fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

	char *fragmentShaderAssembly = _readShaderFile( fragfilename );
	fragmentShaderStrings[0] = fragmentShaderAssembly;
	glShaderSource( fragmentShader, 1, fragmentShaderStrings, NULL );
	glCompileShader( fragmentShader );
	free((void *)fragmentShaderAssembly);

	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, 
		&fragmentCompiled );
	if(fragmentCompiled == GL_FALSE)
	{
		glGetInfoLogARB( fragmentShader, sizeof(str), NULL, str );
		std::cerr << "Fragment shader compile error: " << str << std::endl;
	}

  // Create a program object and attach the compiled shaders
	programObj = glCreateProgram();
	glAttachShader( programObj, vertexShader );
	glAttachShader( programObj, fragmentShader );

  // Link the program object and print out the info log
	glLinkProgram( programObj );
	glGetProgramiv( programObj, GL_LINK_STATUS, &shadersLinked );

	if( shadersLinked == GL_FALSE )
	{
		glGetInfoLogARB( programObj, sizeof(str), NULL, str );
		std::cerr << "Program object linking error: " << str << std::endl;
	}

	std::cout << "Creating shader program [" << programObj << "]: " << std::endl;
	std::cout << "   " << vertfilename << std::endl;
	std::cout << "   " << fragfilename << std::endl;
	
	std::cout << "Attribute positions (-1 if unused in shader): " << std::endl;

	// for debugging
	// glUseProgram( programObj );
	// GLint pos_loc = glGetAttribLocation( programObj, "vertex_position");
	// GLint tex_loc = glGetAttribLocation( programObj, "vertex_tex");
	// GLint normal_loc = glGetAttribLocation( programObj, "vertex_normal");
	// GLint color_loc = glGetAttribLocation( programObj, "vertex_color");
	// GLint attrib3f_loc = glGetAttribLocation( programObj, "vertex_attribute3f");
	// GLint attrib1f_loc = glGetAttribLocation( programObj, "vertex_attribute1f");
	// GLint mvp_loc = glGetUniformLocation( programObj, "MVP");
	// GLint teximage_loc = glGetUniformLocation( programObj, "teximage");
	// std::cout << "   " << "vertex_position = " << pos_loc << std::endl;
	// std::cout << "   " << "vertex_tex = " << tex_loc << std::endl;
	// std::cout << "   " << "vertex_normal = " << normal_loc << std::endl;
	// std::cout << "   " << "vertex_color = " << color_loc << std::endl;
	// std::cout << "   " << "vertex_attribute3f = " << attrib3f_loc << std::endl;
	// std::cout << "   " << "vertex_attributef = " << attrib1f_loc << std::endl;
	// std::cout << "   " << "MVP = " << mvp_loc << std::endl;
	// std::cout << "   " << "teximage = " << teximage_loc << std::endl;
	// glUseProgram( 0 );


	return programObj;
}

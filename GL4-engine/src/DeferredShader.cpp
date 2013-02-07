/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "DeferredShader.h"
#include <iostream>
#include <iomanip>
#include <cstring>
const char *empty = "void VS(){}void GS(int i){}void FS(){}";

gl4::DeferredShader::DeferredShader(const char *filename)
{
	init();

	_generateVertexShader(filename);
	_generateFragmentShader(filename);
	_generateGeometryShader(filename);

	link();
}


gl4::DeferredShader::~DeferredShader()
{
	
}

void gl4::DeferredShader::_generateVertexShader(const char *filename)
{
	const char * source = "#version 430\n\
	layout(location = 0) uniform mat4 Projection;\n\
	layout(location = 1) uniform mat4 ModelTransform;\n\
	layout(location = 2) uniform float applicationTime;\n\
	layout(location = 3) uniform sampler2D texture1;\n\
	layout(location = 4) uniform sampler2D texture2;\n\
	layout(location = 5) uniform sampler2D texture3;\n\
	layout(location = 6) uniform float TessLevel;\n\
	layout(location = 7) uniform bool Wireframe;\n\
	layout(location = 8) uniform bool Lightsource;\n\
	layout(location = 9) uniform bool UseTexture;\n\
	layout(location = 0) in vec3 in_position;\n\
	layout(location = 1) in vec2 in_st;\n\
	layout(location = 2) in vec3 in_normal;\n\
	layout(location = 3) in vec4 in_color;\n\
	layout(location = 4) in vec3 in_attribute3f;\n\
	layout(location = 5) in float in_attribute1f;\n\
	layout(location = 0) out vec2 out_st;\n\
	layout(location = 1) out vec3 out_stp;\n\
	layout(location = 2) out vec4 out_normal;\n\
	layout(location = 3) out vec4 out_color;\n\
	layout(location = 4) out vec4 out_position;\n\
	void VS();\n\
	void main()\n\
	{\n\
		out_st = in_st;\n\
		out_stp = in_position;\n\
		out_normal = normalize(ModelTransform * vec4(in_normal,1));\n\
		out_color = in_color;\n\
		out_position = ModelTransform * vec4(in_position,1);\n\
		gl_Position =  Projection* out_position;\n\
		VS();\n\
	}\n\
	";

	char finalsource[16384];
	strcpy(finalsource, source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"//<!-- VS start -->","//<!-- VS stop -->").c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}

	GLuint shader = _compileShaderSource(GL_VERTEX_SHADER, finalsource, "Vertex");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}
}

void gl4::DeferredShader::_generateFragmentShader(const char *filename)
{
	const char * source = "#version 430\n\
	layout(location = 0) uniform mat4 Projection;\n\
	layout(location = 1) uniform mat4 ModelTransform;\n\
	layout(location = 2) uniform float applicationTime;\n\
	layout(location = 3) uniform sampler2D texture1;\n\
	layout(location = 4) uniform sampler2D texture2;\n\
	layout(location = 5) uniform sampler2D texture3;\n\
	layout(location = 6) uniform float TessLevel;\n\
	layout(location = 7) uniform bool Wireframe;\n\
	layout(location = 8) uniform bool Lightsource;\n\
	layout(location = 9) uniform bool UseTexture;\n\
	layout(location = 0) in vec2 in_st;\n\
	layout(location = 1) in vec3 in_stp;\n\
	layout(location = 2) in vec4 in_normal;\n\
	layout(location = 3) in vec4 in_color;\n\
	layout(location = 4) in vec4 in_position;\n\
	layout(location = 5) in vec3 in_distance_to_edge;\n\
	layout(location = 0) out vec4 diffuse;\n\
	layout(location = 1) out vec4 position;\n\
	layout(location = 2) out vec4 normal;\n\
	void FS();\n\
	void main()\n\
	{\n\
		if(UseTexture)\n\
		{\n\
			diffuse = texture2D(texture1, in_st);\n\
		} else {\n\
			diffuse = in_color;\n\
		}\n\
		position = in_position;\n\
		normal = in_normal;\n\
		// if wireframe is activated, the distance is calculated in pixels\n\
		if(Wireframe)\n\
		{\n\
			float dist = min(min(in_distance_to_edge.x, in_distance_to_edge.y), in_distance_to_edge.z);\n\
			float stepp = 1.0-smoothstep(1.0,2.0,dist);\n\
			diffuse = mix(diffuse,vec4(1,1,1,1), stepp);\n\
		}\n\
		FS();\n\
	}\n\
	";

	char finalsource[16384];
	strcpy(finalsource, source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"//<!-- FS start -->","//<!-- FS stop -->").c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}

	GLuint shader = _compileShaderSource(GL_FRAGMENT_SHADER, finalsource, "Fragment");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}

}

void gl4::DeferredShader::_generateGeometryShader(const char *filename)
{
	const char * source = "#version 430\n\
	layout(triangles) in;\n\
	layout(triangle_strip, max_vertices = 3) out;\n\
	layout(location = 0) uniform mat4 Projection;\n\
	layout(location = 1) uniform mat4 ModelTransform;\n\
	layout(location = 2) uniform float applicationTime;\n\
	layout(location = 3) uniform sampler2D texture1;\n\
	layout(location = 4) uniform sampler2D texture2;\n\
	layout(location = 5) uniform sampler2D texture3;\n\
	layout(location = 6) uniform float TessLevel;\n\
	layout(location = 7) uniform bool Wireframe;\n\
	layout(location = 8) uniform bool Lightsource;\n\
	layout(location = 9) uniform bool UseTexture;\n\
	layout(location = 10) uniform vec2 WindowSize;\n\
	layout(location = 0) in vec2 in_st[3];\n\
	layout(location = 1) in vec3 in_stp[3];\n\
	layout(location = 2) in vec4 in_normal[3];\n\
	layout(location = 3) in vec4 in_color[3];\n\
	layout(location = 4) in vec4 in_position[3];\n\
	layout(location = 0) out vec2 out_st;\n\
	layout(location = 1) out vec3 out_stp;\n\
	layout(location = 2) out vec4 out_normal;\n\
	layout(location = 3) out vec4 out_color;\n\
	layout(location = 4) out vec4 out_position;\n\
	layout(location = 5) out vec3 out_distance_to_edge;\n\
	void GS(int i);\n\
	void main()\n\
	{\n\
	    // taken from Single-Pass Wireframe Rendering\n\
	    vec2 v[3];\n\
	    vec2 p0 = WindowSize * gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;\n\
	    vec2 p1 = WindowSize * gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;\n\
	    vec2 p2 = WindowSize * gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;\n\
	    v[0] = p2-p1;\n\
	    v[1] = p2-p0;\n\
	    v[2] = p1-p0;\n\
	    float area = abs(v[1].x*v[2].y - v[1].y * v[2].x);\n\
	    for(int i = 0; i < 3; i++)\n\
	    {\n\
	        out_st = in_st[i];\n\
	        out_stp = in_stp[i];\n\
	        out_normal = in_normal[i];\n\
	        out_color = in_color[i];\n\
	        out_position = in_position[i];\n\
	        float dist = area/length(v[i]);\n\
	        out_distance_to_edge = vec3(0,0,0);\n\
	        out_distance_to_edge[i] = dist;\n\
	        gl_Position = gl_in[i].gl_Position;\n\
	        GS(i);\n\
	        EmitVertex();\n\
	    }\n\
	}\n\
	";
	
	char finalsource[16384];
	strcpy(finalsource, source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"//<!-- GS start -->","//<!-- GS stop -->").c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}

	GLuint shader = _compileShaderSource(GL_GEOMETRY_SHADER, finalsource, "Geometry");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}
}

GLuint gl4::DeferredShader::_compileShaderSource(GLenum shaderType, const char *source, const char *shaderString)
{
	GLuint shader;
	const char *shaderStrings[1];
	GLint shaderCompiled = GL_FALSE;
	char str[4096]; // For error messages from the GLSL compiler and linker


	shader = glCreateShader( shaderType );

	shaderStrings[0] = source;
	glShaderSource( shader, 1, shaderStrings, NULL );
	glCompileShader( shader );

	glGetShaderiv( shader, GL_COMPILE_STATUS, &shaderCompiled );
	if(shaderCompiled == GL_FALSE)
	{
		glGetInfoLogARB( shader, sizeof(str), NULL, str );
		std::cerr << shaderString << " shader compile error: " << str << std::endl;
		return 0;
	} else {
		return shader;
	}
}

std::string gl4::DeferredShader::_getPartOfString(std::string s, std::string s1, std::string s2) 
{
	unsigned found1 = s.find(s1);

	if (found1 ==std::string::npos)
		return empty;

	unsigned found2 = s.find(s2,found1+1);
	if (found2==std::string::npos)
		return empty;

	return s.substr(found1,found2-found1);
}
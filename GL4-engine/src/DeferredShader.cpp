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

const char *empty = "passthrough();";

const char *fixed_vs_pass1 = "#version 150\n\
uniform mat4 Projection;\n\
uniform mat4 ModelTransform;\n\
uniform float time;\n\
uniform sampler2D texture1;\n\
uniform sampler2D texture2;\n\
uniform sampler2D texture3;\n\
uniform sampler2D texture4;\n\
uniform vec3 uniform_color;\n\
uniform vec2 WindowSize;\n\
in vec3 in_position;\n\
in vec2 in_st;\n\
in vec3 in_normal;\n\
in vec4 in_color;\n\
in vec3 in_attribute3f;\n\
in float in_attribute1f;\n\
out vec2 vs_st;\n\
out vec3 vs_stp;\n\
out vec4 vs_normal;\n\
out vec4 vs_color;\n\
out vec4 vs_position;\n\
void passthrough(){\n\
	vs_st = in_st;\n\
	vs_stp = in_position;\n\
	vs_normal = normalize(ModelTransform * vec4(in_normal,1));\n\
	vs_color = in_color;\n\
	vs_position = ModelTransform * vec4(in_position,1);\n\
	gl_Position =  Projection* vs_position;\n\
}\n\
void main()\n\
{\n\
";

const char *fixed_gs_pass1 = "#version 150\n\
layout(triangles) in;\n\
layout(triangle_strip, max_vertices = 3) out;\n\
uniform mat4 Projection;\n\
uniform mat4 ModelTransform;\n\
uniform float time;\n\
uniform sampler2D texture1;\n\
uniform sampler2D texture2;\n\
uniform sampler2D texture3;\n\
uniform sampler2D texture4;\n\
uniform vec3 uniform_color;\n\
uniform vec2 WindowSize;\n\
in vec2 vs_st[3];\n\
in vec3 vs_stp[3];\n\
in vec4 vs_normal[3];\n\
in vec4 vs_color[3];\n\
in vec4 vs_position[3];\n\
out vec2 st_out;\n\
out vec3 stp_out;\n\
out vec3 fragment_normal_out;\n\
out vec2 gs_st;\n\
out vec3 gs_stp;\n\
out vec4 gs_normal;\n\
out vec4 gs_color;\n\
out vec4 gs_position;\n\
out vec3 gs_distance_to_edge;\n\
void passthrough(){\n\
	vec2 v[3];\n\
	vec2 p0 = WindowSize * gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;\n\
	vec2 p1 = WindowSize * gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;\n\
	vec2 p2 = WindowSize * gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;\n\
	v[0] = p2-p1;\n\
	v[1] = p2-p0;\n\
	v[2] = p1-p0;\n\
	float area = abs(v[1].x*v[2].y - v[1].y * v[2].x);\n\
	for(int i = 0; i < 3; i++){\n\
		gs_st = vs_st[i];\n\
		gs_stp = vs_stp[i];\n\
		gs_normal = vs_normal[i];\n\
		gs_color = vs_color[i];\n\
		gs_position = vs_position[i];\n\
		float dist = area/length(v[i]);\n\
		gs_distance_to_edge = vec3(0,0,0);\n\
		gs_distance_to_edge[i] = dist;\n\
		gl_Position = gl_in[i].gl_Position;\n\
		EmitVertex();\n\
	}\n\
}\n\
void main()\n\
{\n\
";

const char *fixed_fs_pass1 = "#version 150\n\
uniform mat4 Projection;\n\
uniform mat4 ModelTransform;\n\
uniform float time;\n\
uniform sampler2D texture1;\n\
uniform sampler2D texture2;\n\
uniform sampler2D texture3;\n\
uniform sampler2D texture4;\n\
uniform vec3 uniform_color;\n\
uniform vec2 WindowSize;\n\
in vec2 gs_st;\n\
in vec3 gs_stp;\n\
in vec4 gs_normal;\n\
in vec4 gs_color;\n\
in vec4 gs_position;\n\
in vec3 gs_distance_to_edge;\n\
out vec4 diffuse;\n\
out vec4 position;\n\
out vec4 normal;\n\
out vec4 lightmap;\n\
void passthrough(){\n\
	diffuse = gs_color;\n\
	position = gs_position;\n\
	normal = gs_normal;\n\
	lightmap = vec4(0);\n\
}\n\
void wireframe(){\n\
	float dist = min(min(gs_distance_to_edge.x, gs_distance_to_edge.y), gs_distance_to_edge.z);\n\
	float stepp = 1.0-smoothstep(1.0,2.0,dist);\n\
	diffuse = mix(diffuse,vec4(1,1,1,1), stepp);\n\
}\n\
void main()\n\
{\n\
";
const char *fixed_vs_pass2 = "#version 150\n\
uniform mat4 Projection;\n\
uniform mat4 ModelTransform;\n\
uniform sampler2D texture1;\n\
uniform sampler2D texture2;\n\
uniform sampler2D texture3;\n\
uniform sampler2D texture4;\n\
uniform vec3 uniform_color;\n\
uniform vec2 WindowSize = vec2(800,600);\n\
in vec3 in_position;\n\
in vec2 in_st;\n\
in vec3 in_normal;\n\
in vec4 in_color;\n\
in vec3 in_attribute3f;\n\
in float in_attribute1f;\n\
out vec2 st;\n\
out vec3 stp;\n\
out vec4 fragment_normal;\n\
out vec4 fragment_color;\n\
out vec4 fragment_position;\n\
void passthrough(){\n\
	st = in_st;\n\
	stp = in_position;\n\
	fragment_normal = normalize(ModelTransform * vec4(in_normal,1));\n\
	fragment_color = in_color;\n\
	fragment_position = ModelTransform * vec4(in_position,1);\n\
	gl_Position =  Projection* fragment_position;\n\
}\n\
void main()\n\
{\n\
";

const char *fixed_fs_pass2 = "#version 150\n\
uniform mat4 Projection;\n\
uniform mat4 ModelTransform;\n\
uniform sampler2D texture1;\n\
uniform sampler2D texture2;\n\
uniform sampler2D texture3;\n\
uniform sampler2D texture4;\n\
uniform vec3 uniform_color;\n\
uniform vec2 WindowSize = vec2(800,600);\n\
in vec2 st;\n\
in vec3 stp;\n\
in vec4 fragment_normal;\n\
in vec4 fragment_color;\n\
in vec4 fragment_position;\n\
out vec4 diffuse;\n\
void passthrough(){\n\
	diffuse = texture(texture1, st);\n\
}\n\
vec4 bloom(){\n\
	vec4 sum = vec4(0);\n\
	int samples = 20; \n\
	float quality =2.0; \n\
	int diff = (samples - 1) / 2;\n\
	vec2 sizeFactor = vec2(1) / WindowSize * quality;\n\
	\n\
	for (int x = -diff; x <= diff; x++){\n\
		for (int y = -diff; y <= diff; y++){\n\
			vec2 coordinate = st + vec2(x, y) * sizeFactor;\n\
			if (coordinate.x < 0.0 || coordinate.x > 1.0 || coordinate.y < 0.0 || coordinate.y > 1.0 ){\n\
				} else {\n\
					sum += texture(texture4, coordinate);\n\
				}\n\
			}\n\
		}\n\
		return sum / (samples*samples);\n\
	}\n\
	void main()\n\
{\n\
	vec4 in_diffuse = texture(texture1, st);\n\
	vec4 in_position = texture(texture2, st);\n\
	vec4 in_normal = texture(texture3, st);\n\
";

gl4::DeferredShader::DeferredShader(const char *filename, bool pass2)
				{
	init();
	if(!pass2) 
	{
		_generateVertexShader(filename,fixed_vs_pass1);
		_generateFragmentShader(filename,fixed_fs_pass1);
		_generateGeometryShader(filename,fixed_gs_pass1);
	} else {
		_generateVertexShader(filename,fixed_vs_pass2);
		_generateFragmentShader(filename,fixed_fs_pass2);
	}

	// bind the output texture positions
	glBindFragDataLocation(_shaderProgram, 0, "diffuse");
	glBindFragDataLocation(_shaderProgram, 1, "position");
	glBindFragDataLocation(_shaderProgram, 2, "normal");
	glBindFragDataLocation(_shaderProgram, 3, "lightmap");

	// bind all input attributes
	glBindAttribLocation(_shaderProgram, 0, "in_position");
	glBindAttribLocation(_shaderProgram, 1, "in_st");
	glBindAttribLocation(_shaderProgram, 2, "in_normal");
	glBindAttribLocation(_shaderProgram, 3, "in_color");
	glBindAttribLocation(_shaderProgram, 4, "in_attribute3f");
	glBindAttribLocation(_shaderProgram, 5, "in_attribute1f");

	link();
}

gl4::DeferredShader::~DeferredShader()
{
	
}

void gl4::DeferredShader::_generateVertexShader(const char *filename, const char *fixed_source)
{
	//const char * fixed_source = _readShaderFile("GL4-engine/shaders/Deferred1_VS_fixed.glsl");
	char finalsource[16384];
	strcpy(finalsource, fixed_source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"<!-- VS start -->","<!-- VS stop -->",empty).c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}
	strcat(finalsource,"}\n");

	GLuint shader = _compileShaderSource(GL_VERTEX_SHADER, finalsource, "Vertex");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}
}

void gl4::DeferredShader::_generateFragmentShader(const char *filename, const char *fixed_source)
{
	//const char * source = _readShaderFile("GL4-engine/shaders/Deferred1_FS_fixed.glsl");
	char finalsource[16384];
	strcpy(finalsource, fixed_source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"<!-- FS start -->","<!-- FS stop -->",empty).c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}
	strcat(finalsource,"}\n");

	GLuint shader = _compileShaderSource(GL_FRAGMENT_SHADER, finalsource, "Fragment");
	if (shader != 0)
	{
		glAttachShader( _shaderProgram, shader );
	}

}

void gl4::DeferredShader::_generateGeometryShader(const char *filename, const char *fixed_source)
{
	//const char * source = _readShaderFile("GL4-engine/shaders/Deferred1_GS_fixed.glsl");
	char finalsource[16384];
	strcpy(finalsource, fixed_source);
	const char *usersource; 
	if (filename != 0)
	{
		usersource = _getPartOfString(_readShaderFile(filename),"<!-- GS start -->","<!-- GS stop -->",empty).c_str();
		strcat(finalsource,usersource);
	} else {
		strcat(finalsource,empty);
	}
	strcat(finalsource,"}\n");

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
		// print errors 
		glGetInfoLogARB( shader, sizeof(str), NULL, str );
		std::cerr << shaderString << " shader compile error: " << str << std::endl;

		// print the final source
		_printSource(source);
		return 0;
	} else {
		return shader;
	}
}

std::string gl4::DeferredShader::_getPartOfString(std::string s, std::string s1, std::string s2, std::string nothing) 
{
	size_t found1 = s.find(s1);

	if (found1 ==s.npos)
		return nothing;

	size_t found2 = s.find(s2,found1+1);
	if (found2==s.npos)
		return nothing;

	size_t start = found1+s1.length();
	size_t stop = found2-found1-s2.length()-1;

	// if the use source is empty, use the nothing source
	if (stop == 0)
		return nothing;

	return s.substr(start,stop);
}
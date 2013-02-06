/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#version 430

layout(vertices = 3) out;

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 ModelTransform;
layout(location = 3) uniform sampler2D texture1;
layout(location = 4) uniform sampler2D texture2;
layout(location = 5) uniform sampler2D texture3;
layout(location = 6) uniform float TessLevel;

layout(location = 0) in vec3 vs_position[];
layout(location = 1) in vec2 vs_tex[];
layout(location = 2) in vec3 vs_normal[];

layout(location = 0) out vec3 tc_position[];
layout(location = 1) out vec2 tc_tex[];
layout(location = 2) out vec3 tc_normal[];


#define ID gl_InvocationID

void main()
{
	tc_position[ID] = vs_position[ID];
	tc_tex[ID] = vs_tex[ID];
	tc_normal[ID] = vs_normal[ID];
	
	if (ID == 0) {
		gl_TessLevelInner[0] = TessLevel;
		gl_TessLevelOuter[0] = TessLevel;
		gl_TessLevelOuter[1] = TessLevel;
		gl_TessLevelOuter[2] = TessLevel;
	}
	
}
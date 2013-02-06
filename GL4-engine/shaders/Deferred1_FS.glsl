/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#version 430

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 ModelTransform;
layout(location = 3) uniform sampler2D texture1;
layout(location = 4) uniform sampler2D texture2;
layout(location = 5) uniform sampler2D texture3;
layout(location = 6) uniform float TessLevel;
layout(location = 7) uniform bool Wireframe;
layout(location = 8) uniform bool Lightsource;
layout(location = 9) uniform bool UseTexture;

layout(location = 0) in vec2 st;
layout(location = 1) in vec3 stp;
layout(location = 2) in vec4 fragment_normal;
layout(location = 3) in vec4 fragment_color;
layout(location = 4) in vec4 fragment_position;
layout(location = 5) in vec3 distance_to_edge;

layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 position;
layout(location = 2) out vec4 normal;

void main()
{
	if(UseTexture)
	{
		diffuse = texture2D(texture1, st);
	} else {
		diffuse = fragment_color;
	}

	// if wireframe is activated, the distance is calculated in pixels
	if(Wireframe) 
	{
		float dist = min(min(distance_to_edge.x, distance_to_edge.y), distance_to_edge.z);
		if(dist < 1.0) {
			diffuse = vec4(1,1,1,1);
		}
	}

	position = fragment_position;
	normal = fragment_normal;
}
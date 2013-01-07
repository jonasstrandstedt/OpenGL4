/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 ModelTransform;
layout(location = 2) uniform vec3 CameraPosition;

layout(location = 0) in vec2 st[3];
layout(location = 1) in vec3 stp[3];
layout(location = 2) in vec4 fragment_normal[3];
layout(location = 3) in vec4 fragment_color[3];
layout(location = 4) in vec4 fragment_position[3];

layout(location = 0) out vec2 st_out;
layout(location = 1) out vec3 stp_out;
layout(location = 2) out vec4 fragment_normal_out;
layout(location = 3) out vec4 fragment_color_out;
layout(location = 4) out vec4 fragment_position_out;

void main()
{
  for(int i = 0; i < gl_in.length(); i++)
  {
  	gl_Position = gl_in[i].gl_Position;
  	st_out = st[i];
  	stp_out = stp[i];
  	fragment_normal_out = fragment_normal[i];
  	fragment_color_out = fragment_color[i];
  	fragment_position_out = fragment_position[i];
    EmitVertex();
  }
  EndPrimitive();
}
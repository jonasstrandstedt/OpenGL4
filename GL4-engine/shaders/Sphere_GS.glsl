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
layout(location = 3) uniform sampler2D teximage;
layout(location = 4) uniform sampler2D heightimage;
layout(location = 5) uniform float TessLevel;

layout(location = 0) in vec3 te_position[3];
layout(location = 1) in vec2 te_tex[3];
layout(location = 2) in vec3 te_normal[3];

layout(location = 0) out vec2 st_out;
layout(location = 1) out vec3 stp_out;
layout(location = 2) out vec3 fragment_normal_out;
layout(location = 3) out vec3 distance_to_edge;

void main()
{
    st_out = te_tex[0];
    stp_out = te_position[0];
    fragment_normal_out = normalize(te_normal[0]);
    distance_to_edge = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();

    st_out = te_tex[1];
    stp_out = te_position[1];
    fragment_normal_out = normalize(te_normal[1]);
    distance_to_edge = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; 
    EmitVertex();

    st_out = te_tex[2];
    stp_out = te_position[2];
    fragment_normal_out = normalize(te_normal[2]);
    distance_to_edge = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();

    EndPrimitive();
    
}
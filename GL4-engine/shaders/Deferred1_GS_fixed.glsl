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
layout(location = 2) uniform float time;
layout(location = 3) uniform sampler2D texture1;
layout(location = 4) uniform sampler2D texture2;
layout(location = 5) uniform sampler2D texture3;
layout(location = 6) uniform float TessLevel;
layout(location = 7) uniform bool Wireframe;
layout(location = 8) uniform bool Lightsource;
layout(location = 9) uniform bool UseTexture;
layout(location = 10) uniform vec2 WindowSize;

layout(location = 0) in vec2 in_st[3];
layout(location = 1) in vec3 in_stp[3];
layout(location = 2) in vec4 in_normal[3];
layout(location = 3) in vec4 in_color[3];
layout(location = 4) in vec4 in_position[3];

layout(location = 0) out vec2 st_out;
layout(location = 1) out vec3 stp_out;
layout(location = 2) out vec3 fragment_normal_out;

layout(location = 0) out vec2 out_st;
layout(location = 1) out vec3 out_stp;
layout(location = 2) out vec4 out_normal;
layout(location = 3) out vec4 out_color;
layout(location = 4) out vec4 out_position;
layout(location = 5) out vec3 out_distance_to_edge;

void GS(int i);

void main()
{
    // taken from 'Single-Pass Wireframe Rendering'
    vec2 v[3];
    vec2 p0 = WindowSize * gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;
    vec2 p1 = WindowSize * gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;
    vec2 p2 = WindowSize * gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;

    v[0] = p2-p1;
    v[1] = p2-p0;
    v[2] = p1-p0;
    float area = abs(v[1].x*v[2].y - v[1].y * v[2].x);

    for(int i = 0; i < 3; i++)
    {
        out_st = in_st[i];
        out_stp = in_stp[i];
        out_normal = in_normal[i];
        out_color = in_color[i];
        out_position = in_position[i];
        float dist = area/length(v[i]);
        out_distance_to_edge = vec3(0,0,0);
        out_distance_to_edge[i] = dist;
        gl_Position = gl_in[i].gl_Position; 
        GS(i);
        EmitVertex();

    }
    
}
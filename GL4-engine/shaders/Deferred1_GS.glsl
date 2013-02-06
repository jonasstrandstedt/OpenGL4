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
layout(location = 2) in vec4 in_fragment_normal[3];
layout(location = 3) in vec4 in_fragment_color[3];
layout(location = 4) in vec4 in_fragment_position[3];

layout(location = 0) out vec2 st_out;
layout(location = 1) out vec3 stp_out;
layout(location = 2) out vec3 fragment_normal_out;

layout(location = 0) out vec2 out_st;
layout(location = 1) out vec3 out_stp;
layout(location = 2) out vec4 out_fragment_normal;
layout(location = 3) out vec4 out_fragment_color;
layout(location = 4) out vec4 out_fragment_position;
layout(location = 5) out vec3 out_distance_to_edge;

void main()
{
    // taken from 'Single-Pass Wireframe Rendering'
    vec2 p0 = WindowSize * gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;
    vec2 p1 = WindowSize * gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;
    vec2 p2 = WindowSize * gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;
    vec2 v0 = p2-p1;
    vec2 v1 = p2-p0;
    vec2 v2 = p1-p0;
    float area = abs(v1.x*v2.y - v1.y * v2.x);

    out_st = in_st[0];
    out_stp = in_stp[0];
    out_fragment_normal = normalize(in_fragment_normal[0]);
    out_fragment_color = in_fragment_color[0];
    out_fragment_position = in_fragment_position[0];
    out_distance_to_edge = vec3(1, 0, 0);
    out_distance_to_edge = vec3(area/length(v0),0,0);
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    
    out_st = in_st[1];
    out_stp = in_stp[1];
    out_fragment_normal = normalize(in_fragment_normal[1]);
    out_fragment_color = in_fragment_color[1];
    out_fragment_position = in_fragment_position[1];
    out_distance_to_edge = vec3(0, 1, 0);
    out_distance_to_edge = vec3(0,area/length(v1),0);
    gl_Position = gl_in[1].gl_Position; 
    EmitVertex();

    out_st = in_st[2];
    out_stp = in_stp[2];
    out_fragment_normal = normalize(in_fragment_normal[2]);
    out_fragment_color = in_fragment_color[2];
    out_fragment_position = in_fragment_position[2];
    out_distance_to_edge = vec3(0, 0, 1);
    out_distance_to_edge = vec3(0,0,area/length(v2));
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();

    EndPrimitive();
    
}
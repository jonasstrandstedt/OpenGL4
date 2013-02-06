/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#version 430

layout(triangles, equal_spacing, cw) in;

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 ModelTransform;
layout(location = 3) uniform sampler2D texture1;
layout(location = 4) uniform sampler2D texture2;
layout(location = 5) uniform sampler2D texture3;
layout(location = 6) uniform float TessLevel;

layout(location = 0) in vec3 tc_position[];
layout(location = 1) in vec2 tc_tex[];

layout(location = 0) out vec3 te_position;
layout(location = 1) out vec2 te_tex;
layout(location = 2) out vec3 te_normal;

void main()
{
    vec2 t0 = gl_TessCoord.x * tc_tex[0];
    vec2 t1 = gl_TessCoord.y * tc_tex[1];
    vec2 t2 = gl_TessCoord.z * tc_tex[2];
    te_tex = t0 + t1 + t2;

    vec3 p0 = gl_TessCoord.x * tc_position[0];
    vec3 p1 = gl_TessCoord.y * tc_position[1];
    vec3 p2 = gl_TessCoord.z * tc_position[2];

    float dist = 1.0+length(texture2D(texture2, te_tex).xyz)/10.0;

    te_normal = normalize(p0 + p1 + p2);

    te_position = (ModelTransform * vec4(normalize(p0 + p1 + p2)*dist,1)).xyz;

    gl_Position = Projection * vec4(te_position, 1);
}
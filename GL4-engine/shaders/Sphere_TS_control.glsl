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
layout(location = 2) uniform vec3 CameraPosition;
layout(location = 3) uniform sampler2D teximage;
layout(location = 4) uniform sampler2D heightimage;
layout(location = 5) uniform float TessLevelInner;
layout(location = 6) uniform float TessLevelOuter;
layout(location = 7) uniform vec2 screen_size;
layout(location = 8) uniform float lod_factor;

layout(location = 0) in vec3 vs_position[];
layout(location = 1) in vec2 vs_tex[];
layout(location = 2) in vec3 vs_normal[];

layout(location = 0) out vec3 tc_position[];
layout(location = 1) out vec2 tc_tex[];
layout(location = 2) out vec3 tc_normal[];


#define ID gl_InvocationID


vec4 project(vec4 vertex){
	vec4 result = Projection * ModelTransform * vertex;
	result /= result.w;
	return result;
}
vec2 screen_space(vec4 vertex){
	return (clamp(vertex.xy, -1.3, 1.3)+1) * (screen_size*0.5);
}
float level(vec2 v0, vec2 v1){
	return clamp(distance(v0, v1)/lod_factor, 1, 64);
}
bool offscreen(vec4 vertex){
	if(vertex.z < -0.5){
		return true;
	}   
	return any(
		lessThan(vertex.xy, vec2(-1.7)) ||
		greaterThan(vertex.xy, vec2(1.7))
		);  
}


void main()
{
	tc_position[ID] = vs_position[ID];
	tc_tex[ID] = vs_tex[ID];
	tc_normal[ID] = vs_normal[ID];
	/*
	if (ID == 0) {
		gl_TessLevelInner[0] = TessLevelInner;
		gl_TessLevelOuter[0] = TessLevelOuter;
		gl_TessLevelOuter[1] = TessLevelOuter;
		gl_TessLevelOuter[2] = TessLevelOuter;
	}
	*/
	
	if(ID == 0){
		vec4 v0 = project(gl_in[0].gl_Position);
		vec4 v1 = project(gl_in[1].gl_Position);
		vec4 v2 = project(gl_in[2].gl_Position);
		vec4 v3 = project(gl_in[3].gl_Position);

		if(all(bvec3(
			offscreen(v0),
			offscreen(v1),
			offscreen(v2)
			))){
			gl_TessLevelInner[0] = 0;
			gl_TessLevelOuter[0] = 0;
			gl_TessLevelOuter[1] = 0;
			gl_TessLevelOuter[2] = 0;
		}
		else{
			vec2 ss0 = screen_space(v0);
			vec2 ss1 = screen_space(v1);
			vec2 ss2 = screen_space(v2);
			vec2 ss3 = screen_space(v3);

			float e0 = level(ss1, ss2);
			float e1 = level(ss0, ss1);
			float e2 = level(ss2, ss0);

			gl_TessLevelInner[0] = mix(e0, e2, 0.5);
			gl_TessLevelOuter[0] = e0;
			gl_TessLevelOuter[1] = e1;
			gl_TessLevelOuter[2] = e2;

			gl_TessLevelInner[0] = TessLevelInner;
			gl_TessLevelOuter[0] = TessLevelOuter;
			gl_TessLevelOuter[1] = TessLevelOuter;
			gl_TessLevelOuter[2] = TessLevelOuter;
		}
	}
	
}
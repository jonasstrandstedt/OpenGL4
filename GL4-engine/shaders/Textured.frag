#version 430

layout(location = 0) uniform mat4 MVP;
layout(location = 1) uniform sampler2D teximage;

layout(location = 0) in vec2 st;
layout(location = 1) in vec3 stp;
layout(location = 2) in vec3 fragment_normal;
layout(location = 3) in vec4 fragment_color;

layout(location = 0) out vec4 finalColor;

void main()
{
	finalColor = texture2D(teximage, st);
	//finalColor = vec4(1,0,0,1);
	//finalColor = fragment_color;
}
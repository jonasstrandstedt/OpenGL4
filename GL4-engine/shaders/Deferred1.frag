#version 430

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 ModelTransform;
layout(location = 2) uniform vec3 CameraPosition;
layout(location = 3) uniform sampler2D teximage;

layout(location = 0) in vec2 st;
layout(location = 1) in vec3 stp;
layout(location = 2) in vec4 fragment_normal;
layout(location = 3) in vec4 fragment_color;
layout(location = 4) in vec4 fragment_position;

layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 position;
layout(location = 2) out vec4 normal;

void main()
{

	diffuse = fragment_color;
	position = fragment_position;
	normal = fragment_normal;
}
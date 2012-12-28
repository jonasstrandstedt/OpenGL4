#version 430

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 ModelTransform;
layout(location = 2) uniform vec3 CameraPosition;


layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_tex;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec4 vertex_color;
layout(location = 4) in vec3 vertex_attribute3f;
layout(location = 5) in float vertex_attribute1f;

layout(location = 0) out vec2 st;
layout(location = 1) out vec3 stp;
layout(location = 2) out vec4 fragment_normal;
layout(location = 3) out vec4 fragment_color;
layout(location = 4) out vec4 fragment_position;

void main()
{

	st = vertex_tex;
	stp = vertex_position;
	fragment_normal = normalize(ModelTransform * vec4(vertex_normal,1));
	fragment_color = vertex_color;
	fragment_position = Projection * ModelTransform * vec4(vertex_position,1);

	gl_Position =  fragment_position;

}
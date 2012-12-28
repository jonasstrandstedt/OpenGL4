#version 430

layout(location = 0) uniform mat4 Projection;
layout(location = 1) uniform mat4 ModelTransform;
layout(location = 2) uniform vec3 CameraPosition;
layout(location = 3) uniform sampler2D texture_diffuse;
layout(location = 4) uniform sampler2D texture_position;
layout(location = 5) uniform sampler2D texture_normal;

layout(location = 0) in vec2 st;
layout(location = 1) in vec3 stp;
layout(location = 2) in vec4 fragment_normal;
layout(location = 3) in vec4 fragment_color;

layout(location = 0) out vec4 finalColor;

void main()
{
	vec4 diffuse = texture2D(texture_diffuse, st);
	vec4 position = texture2D(texture_position, st);
	vec4 normal = texture2D(texture_normal, st);

	vec3 light = vec3(50,100,50);
	vec3 lightDir = light - position.xyz ;

	normal = normalize(normal);
	lightDir = normalize(lightDir);

	vec3 eyeDir = normalize(CameraPosition-position.xyz);
	vec3 vHalfVector = normalize(lightDir.xyz+eyeDir);

	vec3 v1 =  max(dot(normal.xyz,lightDir),0) * diffuse.xyz;
	float f = pow(max(dot(normal.xyz,vHalfVector),0.0), 100) * 1.5;
	finalColor = vec4(v1+f,1);
}
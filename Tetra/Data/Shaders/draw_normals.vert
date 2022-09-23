#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

out VARYING
{
	vec3 position;
	vec3 normal;
}oData;

uniform mat4 worldMat;
uniform mat3 normalMat;

void main()
{
	vec4 worldSpacePosition = worldMat * vec4(position, 1.0);

	oData.position = worldSpacePosition.xyz;
	oData.normal = normalMat * normal;

	gl_Position = worldSpacePosition;
}
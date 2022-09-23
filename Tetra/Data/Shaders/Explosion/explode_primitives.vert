#version 330 core

//IN
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

//OUT
out Varying
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
} oData;

//UNIFORM
uniform mat4 worldMat;
uniform mat3 normalMat;

void main()
{
	vec4 position = worldMat * vec4(position, 1.0);
	oData.position = position.xyz;
	oData.texCoord = texCoord;
	oData.normal = normalMat * normal;

	gl_Position = position;
};

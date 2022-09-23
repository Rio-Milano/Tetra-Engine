#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTextureCord;
layout(location = 2) in vec3 inNormal;

out Varying
{
	vec2 textureCord;
	vec3 normal;
	vec3 position;
} outData;

uniform mat3 normalMat;

void main()
{
	//pass vertex data to geometry shader
	outData.textureCord = inTextureCord;
	outData.normal = normalMat * inNormal;
	outData.position = inPos;

	gl_Position = vec4(inPos, 1.0);
}
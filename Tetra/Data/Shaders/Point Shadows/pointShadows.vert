#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoord;

out VARYING
{	vec2 texCoord;
}oData;

uniform mat4 worldMat;

void main()
{
	//pass the texture cord to the geometry shader
	oData.texCoord = TextureCoord;
	//transform the position to world space
	gl_Position = worldMat * vec4(Position, 1.0);
}
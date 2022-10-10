#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoord;

out VARYING
{	vec2 texCoord;
}oData;

uniform mat4 worldMat;

void main()
{
	oData.texCoord = TextureCoord;
	gl_Position = worldMat * vec4(Position, 1.0);
}
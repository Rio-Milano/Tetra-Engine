#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTextureCord;

out vec2 textureCord;

uniform mat4 worldMat;

void main()
{
	textureCord = inTextureCord;

	gl_Position = worldMat * vec4(inPos, 1.0);//what is prespective division? 
}
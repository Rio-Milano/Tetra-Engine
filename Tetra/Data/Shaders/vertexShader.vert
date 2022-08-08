#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTextureCord;

out vec2 textureCord;

uniform mat4 worldMat;
uniform mat4 Projection_X_View;

void main()
{
	textureCord = inTextureCord;

	gl_Position = Projection_X_View * worldMat * vec4(inPos, 1.0);//what is prespective division? 
}
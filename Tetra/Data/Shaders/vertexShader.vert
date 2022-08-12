#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTextureCord;
layout(location = 2) in vec3 inNormal;

out vec2 textureCord;
out vec3 normal;
out vec3 position;

uniform mat4 worldMat;
uniform mat4 Projection_X_View;

void main()
{
	textureCord = inTextureCord;
	normal = inNormal;
	position = inPos;

	gl_Position = Projection_X_View * worldMat * vec4(inPos, 1.0);
}
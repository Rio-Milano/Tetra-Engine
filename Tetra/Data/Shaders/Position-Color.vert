#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 worldMat;//not used

layout(std140) uniform Matricies
{
	mat4 Projection_X_View;
};

out vec3 varying_color;

void main()
{
	gl_Position = Projection_X_View * worldMat * vec4(position, 1.0);
	varying_color = color;
};
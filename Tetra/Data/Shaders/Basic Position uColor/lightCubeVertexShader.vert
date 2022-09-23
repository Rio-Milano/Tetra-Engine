#version 330 core

layout(location = 0) in vec3 position;

layout(std140) uniform Matricies
{
	mat4 Projection_X_View;
};

uniform mat4 worldMat;

void main()
{
	gl_Position = Projection_X_View * worldMat * vec4(position, 1.0);
}
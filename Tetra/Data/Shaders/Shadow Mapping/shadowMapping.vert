#version 330 core

layout(location = 0) in vec3 Position;

uniform mat4 Light_Projection_X_View;
uniform mat4 worldMat;

void main()
{
	gl_Position = Light_Projection_X_View * worldMat * vec4(Position, 1.0);
}
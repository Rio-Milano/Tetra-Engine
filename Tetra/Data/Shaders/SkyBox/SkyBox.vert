#version 330 core

layout(location = 0) in vec3 inPosition;

out vec3 varying_texture_coord;

uniform mat4 Projection_X_View;


void main()
{
	varying_texture_coord = inPosition;
	vec4 position = Projection_X_View * vec4(inPosition, 1.0);
	position = position.xyww;
	gl_Position = position;
};
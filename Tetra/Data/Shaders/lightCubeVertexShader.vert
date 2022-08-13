#version 330 core

layout(location = 0) in vec3 position;

out vec3 varying_color;

uniform mat4 Projection_X_View;
uniform mat4 world;

void main()
{

	gl_Position = Projection_X_View * world * vec4(position, 1.0);

}
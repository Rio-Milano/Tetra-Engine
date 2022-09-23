#version 330 core

in vec3 varying_color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(varying_color, 1.0);
}
#version 330 core

in vec3 varying_texture_coord;

out vec4 FragColor;

uniform samplerCube cubeMap;

void main()
{
	vec3 fragColor = texture(cubeMap, varying_texture_coord).xyz;
	
	FragColor = vec4(fragColor, 1.0);
};
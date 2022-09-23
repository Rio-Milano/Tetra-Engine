#version 330 core

//IN
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;

//OUT
out vec2 varying_texCoord;

void main()
{
	varying_texCoord = inTexCoord;
	//Positions are in clip space
	gl_Position = vec4(inPos, 1.0);
}
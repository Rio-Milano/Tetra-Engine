#version 330 core

//IN
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;

//OUT
out vec2 varying_texCoord;

//UNIFORMS
uniform mat4 worldMat;
layout(std140) uniform Matricies
{
	mat4 Projection_X_View;
};


void main()
{
	varying_texCoord = inTexCoord;
	gl_Position = Projection_X_View * worldMat * vec4(inPos, 1.0);
}
#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;

out vec2 varying_texCoord;

uniform mat4 worldMat;
uniform mat4 Projection_X_View;

void main()
{
	varying_texCoord = inTexCoord;
	
	gl_Position = Projection_X_View * worldMat * vec4(inPos, 1.0);
}
#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoord;

out vec2 varying_texture_coord;

uniform mat4 Light_Projection_X_View;
uniform mat4 worldMat;

void main()
{
	varying_texture_coord = TextureCoord;
	gl_Position = Light_Projection_X_View * worldMat * vec4(Position, 1.0);
}
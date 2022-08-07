#version 330 core

in vec2 textureCord;

out vec4 fragColor;

uniform sampler2D textureSampler;


void main()
{
	vec2 texCord = textureCord;
	texCord.x = -texCord.x +1.0;//invert over yaxis
	

	fragColor = texture(textureSampler, texCord);
}
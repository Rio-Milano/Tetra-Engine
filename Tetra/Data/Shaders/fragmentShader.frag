#version 330 core

in vec2 textureCord;
in vec3 normal;
in vec3 position;

out vec4 fragColor;

uniform sampler2D textureSampler;


vec3 ambientColor = vec3(1.0, 1.0, 1.0);
float ambientColorIntensity = 0.5;

//point light
uniform vec3 lightPos = vec3(4.0, 0.0, 4.0);
vec3 lightColor = vec3(0.0, 1.0, 1.0);
float lightColorIntensity = 0.7;

void main()
{
	vec3 finalAmbient = ambientColor * ambientColorIntensity;
	
	vec3 re_normal = normalize(normal);
	vec3 lightDirection  = normalize(lightPos - position);
	vec3 finalLightColor = max(dot(lightDirection, re_normal), 0) * lightColor * lightColorIntensity;

	vec3 finalFragmentColor = (finalAmbient + finalLightColor) * texture(textureSampler, textureCord).xyz;

	fragColor = vec4(finalFragmentColor, 1.0); //texture(textureSampler, textureCord);
}
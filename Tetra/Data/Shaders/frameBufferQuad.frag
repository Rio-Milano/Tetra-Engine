#version 330 core


in vec2 varying_texCoord;

out vec4 FragColor;

struct Material
{
	sampler2D diffuseMap;
	bool hasDiffuseMap;
	vec3 defaultDiffuseColor;
};


uniform Material material;

void main()
{
	FragColor = vec4(texture(material.diffuseMap, varying_texCoord).xyz, 1.0);//note that this includes the alpha value of the texture so blending is applied if enabled
}
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
	vec4 fragColor = texture(material.diffuseMap, varying_texCoord);
	if(fragColor.a < 0.1)
		discard;

	FragColor = fragColor;
}
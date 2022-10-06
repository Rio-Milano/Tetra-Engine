#version 330 core

in vec2 varying_texture_coord;

out vec4 FragColor;

struct Material
{
	sampler2D diffuseMap;
	bool hasDiffuseMap;
	vec3 defaultDiffuseColor;
	
	sampler2D specularMap;
	bool hasSpecularMap;
	vec3 defaultSpecularColor;
	bool discardLowAlphaFragment;

	sampler2D emissionMap;
	bool hasEmissionMap;
	float emissionRange;

	float ambientIntensity;
	float specularIntensity;

	bool mapToEnviroment;
	bool hasCubeMap;
	samplerCube cubeMap;
	int reflectionType;//0 = reflection, 1 = refraction
};

uniform Material material;

void main()
{
	if(material.discardLowAlphaFragment)
	{
		if(material.hasDiffuseMap)
		{
			if(texture(material.diffuseMap, varying_texture_coord).a < 0.1)
			{
				discard;
			}
		}
	}

	FragColor = vec4(1.0);
}
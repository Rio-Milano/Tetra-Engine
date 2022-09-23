#version 330 core

//IN
in vec2 varying_texCoord;

//OUT
out vec4 FragColor;

//STRUCTS
struct Material
{
	sampler2D diffuseMap;
	bool hasDiffuseMap;
	vec3 defaultDiffuseColor;
};

//UNIFORM
uniform Material material;

void main()
{
	//Sample the diffuse map
	vec4 fragColor = texture(material.diffuseMap, varying_texCoord);
	
	//if alpha value is very low then discard the fragment
	if(fragColor.a < 0.1)
		discard;

	FragColor = fragColor;
}
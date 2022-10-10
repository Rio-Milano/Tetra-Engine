#version 330 core

in VARYING
{
	vec4 FragPos;
	vec2 texCoord;
}iData;


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
//STRUCTS
struct Light
{
	/*
	LIGHT TYPE :  = Directional
	LIGHT TYPE : 2 = Point
	LIGHT TYPE : 3 = Spot
	*/

	vec3 position; 
	float range;

	vec3 color;	
	float innerCutOffAngle;

	vec3 direction;	
	float intensity;
	
	mat4 lightSpace;

	float outerCutOffAngle;	
	int type; 
	bool inUse;
	float farPlane;

};


#define NUMBER_OF_LIGHTS 10

layout(std140) uniform Lights
{
	Light lights[NUMBER_OF_LIGHTS];
};

uniform Material material;
uniform int lightIndex;

/*

Because the Omnidirectional Shadow algorithm uses perspective projection we will end up getting quadratic depth. Working arround 
quadratic depth makes our calculations more complex
so here we override any pre depth test by calculating the linear depth ourselves

*/

void main()
{
	//if discard low alpha values
	if(material.discardLowAlphaFragment && material.hasDiffuseMap)
	{
		if(texture(material.diffuseMap, iData.texCoord).a < 0.1)
			discard;
	}

	//get the depth of the fragment in the scene
	float lightFragmentLength = length(iData.FragPos.xyz - lights[lightIndex].position);
	//put the depth in range of 0->1
	float depth = lightFragmentLength  / lights[lightIndex].farPlane;
	//override predepth test to set our own depth
	gl_FragDepth = depth;

}
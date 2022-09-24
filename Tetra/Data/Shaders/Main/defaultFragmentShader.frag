#version 330 core

//IN
in Varying
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
} inData;


//OUT
out vec4 FragColor;

//STRUCTS
struct Light
{
	/*
	LIGHT TYPE :  = Directional
	LIGHT TYPE : 2 = Point
	LIGHT TYPE : 3 = Spot
	*/

	vec3 position;
	vec3 color;
	vec3 direction;
	float intensity;
	float 
		range,
		innerCutOffAngle,
		outerCutOffAngle;
	int type;
	bool inUse;
};
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


//MACROs
#define SPECULAR_POWER 32
#define NUMBER_OF_LIGHTS 10

//UNIFORM
layout(std140) uniform Lights
{
	Light lights[NUMBER_OF_LIGHTS];
};
layout(std140) uniform World
{
	vec3 cameraPosition;
	float time;
};
uniform Material material;
uniform float fromRefractiveIndex;
uniform float toRefractiveIndex;


//FUNCTION PROTOTYPES

float CalculateAttenuation(int i);
float CalculateDiffuse(vec3 dirToLight, vec3 normal);
float CalculateSpecular(int i, vec3 normal);

vec3 CalculateDirectionalLight(int i, vec3 normal);
vec3 CalculatePointLight(int i, vec3 normal);
vec3 CalculateSpotLight(int i, vec3 normal);

vec3 GetFragmentDiffuse();
vec3 GetFragmentSpecular();
vec3 GetFragmentEmission();

vec3 ProcessReflectiveFragment(vec3 normal);
bool ProcessLowAlphaFragment();
bool ProcessEmissionFragment();

void main()
{
	if(ProcessLowAlphaFragment()) return;

	vec3 normal = normalize(inData.normal);//re-normalize from scaling
	
	vec3 reflectionContribution = ProcessReflectiveFragment(normal);

	if(ProcessEmissionFragment()) return;

	vec3 finalColor;

	for(int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{

		if(!lights[i].inUse)
			continue;

		switch(lights[i].type)
		{
			case 0://DIRECTIONAL
				finalColor += CalculateDirectionalLight(i, normal);
				break;

			case 1://POINT
				finalColor += CalculatePointLight(i, normal);
				break;

			case 2://SPOT
				finalColor += CalculateSpotLight(i, normal);
				break;
			default:
				break;
		};

	};

	if(material.mapToEnviroment && material.hasCubeMap)
		FragColor = mix(vec4(finalColor, 1.0), vec4(reflectionContribution, 1.0), 0.1);
	else
		FragColor = vec4(finalColor, 1.0);
}

bool ProcessEmissionFragment()
{
	//emission calculations
	float distanceToEmission = distance(cameraPosition, inData.position);
	float attenation =  1.0 - smoothstep(0, material.emissionRange, distanceToEmission);

	vec3 emissionColor =  GetFragmentEmission() * attenation;

	if(dot(emissionColor, emissionColor) > 0.0)
	{
		FragColor = vec4(emissionColor, 1.0);
		return true;
	}
	return false;
}

bool ProcessLowAlphaFragment()
{
	if(material.discardLowAlphaFragment)
	{
		if(material.hasDiffuseMap)
		{
			if(texture(material.diffuseMap, inData.texCoord).a < 0.1)
			{
				discard;
				return true;
			}
		}
	};
	return false;
}

vec3 ProcessReflectiveFragment(vec3 normal)
{
	if(material.mapToEnviroment && material.hasCubeMap)
	{
		vec3 I = normalize(inData.position - cameraPosition);
		vec3 R = vec3(0.0);

		switch(material.reflectionType)
		{
			case 0://reflection
				R = reflect(I, normal);
				break;

			case 1://refraction
				R = refract(I, normal, fromRefractiveIndex / toRefractiveIndex);
				break;

			default:
				break;
				
		};
		return texture(material.cubeMap, R).xyz;
	};
	return vec3(0);
}

vec3 GetFragmentDiffuse()
{
	if(material.hasDiffuseMap)
		return texture(material.diffuseMap, inData.texCoord).xyz;
	else
		return material.defaultDiffuseColor;
};

vec3 GetFragmentSpecular()
{
	if(material.hasSpecularMap)
		return texture(material.specularMap, inData.texCoord).xyz;
	else
	{
		if(material.hasDiffuseMap)
			return GetFragmentDiffuse();
		else
			return material.defaultSpecularColor;
	}
};

vec3 GetFragmentEmission()
{
	if(material.hasEmissionMap)
		return texture(material.emissionMap, inData.texCoord).xyz;
	else	
		return vec3(0.0);
}


float CalculateAttenuation(int i)
{
	float distanceToLight = distance(lights[i].position, inData.position);//find distance between light position and surface position
	return 1.0 - smoothstep(0, lights[i].range, distanceToLight);//use hermite interpolation to get a strength of light basssed on distance
}



float CalculateDiffuse(vec3 dirToLight, vec3 normal)
{
	return max(dot(dirToLight, normal), 0.0);//calculate a positive angle between a vector and surface normal to find access to light
}



float CalculateSpecular(int i, vec3 normal)
{
	vec3 dirToCamera = normalize(cameraPosition - inData.position);//Surface -> Eye
	vec3 dirToLight = normalize(lights[i].position - inData.position);//Surface -> Light

	vec3 reflected = reflect(-dirToLight, normal);//reflect Surface->Light over surface normal

	return material.specularIntensity * pow(CalculateDiffuse(dirToCamera, reflected), SPECULAR_POWER);//specular calculation
}



vec3 CalculateDirectionalLight(int i, vec3 normal)
{
	//Light and Fragment Color
	vec3 lightColor = lights[i].color * lights[i].intensity; // calculate overall light color
	vec3 fragmentColor = GetFragmentDiffuse();
	vec3 specularColor = GetFragmentSpecular();

	//Ambient Color
	vec3 ambient = fragmentColor * material.ambientIntensity;

	//Diffuse color
	vec3 inverseLightDirection = normalize(-lights[i].direction);//get vector pointing towards directional light
	vec3 diffuse = fragmentColor * CalculateDiffuse(inverseLightDirection, normal);//find angle netween surface normal and inverse directional light
	
	//Specular color
	vec3 specular = specularColor * material.specularIntensity * CalculateSpecular(i, normal);//calculate specular

	return (ambient + diffuse + specular) * lightColor;//calculate the final light color
};



vec3 CalculatePointLight(int i, vec3 normal)
{
	vec3 lightColor = lights[i].color * lights[i].intensity;//calculate overall light color
	vec3 fragmentColor = GetFragmentDiffuse();
	vec3 specularColor = GetFragmentSpecular();

	vec3 dirToLight = normalize(lights[i].position - inData.position);//get vector from surface to light
	float attenuation = CalculateAttenuation(i);//calculate light fall off

	vec3 ambient = fragmentColor * material.ambientIntensity * attenuation;

	vec3 diffuse = fragmentColor * CalculateDiffuse(dirToLight, normal) * attenuation;//calculate light reaching surface and apply fall off
	
	vec3 specular =  specularColor * material.specularIntensity * CalculateSpecular(i, normal) * attenuation;//calculate specular on surface and apply fall off

	return (ambient + diffuse + specular) * lightColor;//final calculation
};

vec3 CalculateSpotLight(int i, vec3 normal)
{
	vec3 lightColor = lights[i].color * lights[i].intensity;//overall light color
	vec3 fragmentColor = GetFragmentDiffuse();
	vec3 specularColor = GetFragmentSpecular();

	vec3 dirToLight = normalize(lights[i].position - inData.position);//vector from surface to light
	float theta = dot(-dirToLight, normalize(lights[i].direction));//calculate angle of -light direction and the fragment->light
	float attenuation = CalculateAttenuation(i);//calculte light fall off
	
	vec3 ambient = fragmentColor * material.ambientIntensity * attenuation;

	if(theta >= lights[i].outerCutOffAngle)//if the angle of the fragment is inside of the outter cutoff
	{
		vec3 diffuse = fragmentColor * CalculateDiffuse(dirToLight, normal) * attenuation;
		
		vec3 specular = specularColor * material.specularIntensity * CalculateSpecular(i, normal) * attenuation;//calculate reachable specular and apply fall off

		//calculate the angle between the inner and outer cutoff
		float epsilon = lights[i].innerCutOffAngle - lights[i].outerCutOffAngle;
		
		//calculate the angle between theta and outer cutoff angle which gives how far along theta is past of before the outer angle then divide it by epsilon
		
		float I = smoothstep(0.0, 1.0, (theta - lights[i].outerCutOffAngle) / epsilon);
		//if fragment is inbetween the inner and outer cutoff I should be 0->1
		//if fragment is inside inner cutoff I would be > 1
		//if fragment is outside outer cutoff I is < 1
		//hermite will clamp but smoother

		diffuse *= I;
		specular *= I;

		//final calculation
		return (ambient + diffuse + specular) * lightColor;
	}


	return ambient * lightColor;

};
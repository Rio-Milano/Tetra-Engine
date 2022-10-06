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
	float range;

	vec3 color;	
	float innerCutOffAngle;

	vec3 direction;	
	float intensity;
	
	mat4 lightSpace;

	float outerCutOffAngle;	
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
#define SPECULAR_POWER 64
#define NUMBER_OF_LIGHTS 10

//UBO
layout(std140) uniform Lights
{
	Light lights[NUMBER_OF_LIGHTS];
};
layout(std140) uniform World
{
	vec3 cameraPosition;
	float time;
};

uniform sampler2D shadowMaps[NUMBER_OF_LIGHTS];
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
float ProcessShadow(int i, vec4 posInLightSpace);

void main()
{
	if(ProcessLowAlphaFragment()) return;

	vec3 normal = normalize(inData.normal);//re-normalize from scaling

	if(ProcessEmissionFragment()) return;

	vec3 finalColor = vec3(0.0);

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
	vec3 reflectionContribution = ProcessReflectiveFragment(normal) + GetFragmentDiffuse()*material.ambientIntensity;

	if(material.mapToEnviroment && material.hasCubeMap)
		FragColor = vec4(mix(finalColor, reflectionContribution, 0.1), 1.0);
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
	//Phong Specular
//	vec3 dirToCamera = normalize(cameraPosition - inData.position);//Surface -> Eye
//	vec3 dirToLight = normalize(lights[i].position - inData.position);//Surface -> Light
//
//	vec3 reflected = reflect(-dirToLight, normal);//reflect Surface->Light over surface normal
//
//	return material.specularIntensity * pow(max(0.0, CalculateDiffuse(dirToCamera, reflected)), SPECULAR_POWER);//specular calculation

	//Blin-Phong Specular
	vec3 dirToCamera = cameraPosition - inData.position;
	vec3 dirToLight = lights[i].position - inData.position;
	vec3 halfwayVector = normalize(dirToCamera + dirToLight);
	return material.specularIntensity * pow(max(0.0, dot(normal, halfwayVector)), SPECULAR_POWER);//specular calculation

}

/*
Shadow calculations for directional lighting using orthographic projection
*/
float ProcessShadow(int i, vec4 posInLightSpace, vec3 normal)
{
	vec3 projCoords = posInLightSpace.xyz;//use -> if using perspective projection so scaling can take effect: / posInLightSpace.w; 

	//Convert NDC to range of [0, 1] so that x,y is in texture space and z is in depth space
	projCoords = projCoords * 0.5 + 0.5;

	//if the depth is past the far plane then ignore any shadow
	if(projCoords.z > 1.0)
		return 0.0;

	//get the depth of the fragment in light from the shadow map
	float depthOfFragmentInLight = texture(shadowMaps[i], projCoords.xy).r;
	float depthOfCurrentFragmentInLightSpace = projCoords.z;

	/*
	When we have alignment of the normal and direction to the light we use a very small bias to offset the imprecision of floating point math//When the normal 
	is very steep to the direction to the light we use a larger bias this is because when we render fragments aligned with the light the differance between fragment
	placement is small so floating point values are closer togeather When the fragment normal and light source has a steep angle fragments
	placement in the world can vairy a lot resulting in large floating point imprecision

	So the larger the slope the larger the bias
	*/

	float alignedBias = 0.001;
	float steepSlopeBias = 0.01;

	float bias = max(steepSlopeBias * (1.0 - dot(normal, -lights[i].direction)), alignedBias);

	//Start with a shadow value of 0 meaning fragment is not in shadow
	float shadow = 0.0;

	//get the size of texles in the shadow map
    vec2 texelSize = 1.0 / textureSize(shadowMaps[i], 0);
    vec2 centreTexel = vec2(projCoords.x, projCoords.y);

	/*
	Here we use Percentage-Closer Filtering where we also sample from the depth of surrounding texles
	and include them if they are in shadow. Then we average the shadow by sample count.
	*/

	//divide shadow by how many samples were taken
	int sampleCount = 0;

	//loop arround the texel in the depth map
	for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
			vec2 texelOffset = texelSize * vec2(x, y);
			vec2 newSampleCoord = centreTexel + texelOffset;
			float depth = texture(shadowMaps[i], newSampleCoord).r;
			shadow += depthOfCurrentFragmentInLightSpace - bias > depthOfFragmentInLight ? 1.0 : 0.0;
			sampleCount+=1;
		}    
    }
    shadow /= sampleCount;
	return shadow;
};

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

	float shadowContribution = 1.0 - ProcessShadow(i, lights[i].lightSpace * vec4(inData.position, 1.0), normal);

	return (ambient + shadowContribution * (diffuse + specular)) * lightColor;
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
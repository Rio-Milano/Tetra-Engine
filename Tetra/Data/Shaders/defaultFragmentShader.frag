#version 330 core

/*
 IN IN IN IN IN IN IN IN IN IN IN IN IN IN IN IN
*/
in vec2 varying_textureCord;
in vec3 varying_normal;
in vec3 varying_position;





/*
 OUT OUT OUT OUT OUT OUT OUT OUT OUT OUT OUT OUT
*/
out vec4 FragColor;





/*
 STRUCT STRUCT STRUCT STRUCT STRUCT STRUCT STRUCT
*/
struct Light
{
	vec3 
		position,
		color,
		direction;
	
	float 
		intensity,
		range,
		cutOffAngle;

	uint type;

	bool inUse;

	/*
	LIGHT TYPE : 0 = Amient
	LIGHT TYPE : 1 = Directional
	LIGHT TYPE : 2 = Point
	LIGHT TYPE : 3 = Spot
	*/
};

struct Material
{
	sampler2D diffuseMap;
	bool hasDiffuseMap;
	vec3 defaultDiffuseColor;
	
	sampler2D specularMap;
	bool hasSpecularMap;
	vec3 defaultSpecularColor;

	float ambientIntensity;
	float specularIntensity;

};



/*
MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
*/
#define NUMBER_OF_LIGHTS 10
#define SPECULAR_POWER 32




/*
 UNIFORM UNIFORM UNIFORM UNIFORM UNIFORM UNIFORM
*/
uniform Material material;
uniform Light lights [NUMBER_OF_LIGHTS];
uniform vec3 cameraPosition;


/*
FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS 
*/
float CalculateAttenuation(int i);
float CalculateDiffuse(vec3 dirToLight, vec3 normal);
float CalculateSpecular(int i, vec3 normal);

vec3 CalculateDirectionalLight(int i, vec3 normal);
vec3 CalculatePointLight(int i, vec3 normal);
vec3 CalculateSpotLight(int i, vec3 normal);

vec3 GetFragmentDiffuse();
vec3 GetFragmentSpecular();




void main()
{
	vec3 normal = normalize(varying_normal);//check on why normal must be renormalized... scaling in vertex shader??
	vec3 finalColor;

	for(int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{

		if(!lights[i].inUse)
			continue;

		switch(lights[i].type)
		{
			case 0U://DIRECTIONAL
				finalColor += CalculateDirectionalLight(i, normal);
				break;

			case 1U://POINT
				finalColor += CalculatePointLight(i, normal);
				break;

			case 2U://SPOT
				finalColor  += CalculateSpotLight(i, normal);
				break;
			default:
				break;
		};

	};

	FragColor = vec4(finalColor, 1.0);

}


vec3 GetFragmentSpecular()
{
	if(material.hasSpecularMap)
		return texture(material.specularMap, varying_textureCord).xyz;
	else
	{
		if(material.hasDiffuseMap)
			return GetFragmentDiffuse();
		else
			return material.defaultSpecularColor;
	}
};


vec3 GetFragmentDiffuse()
{
	if(material.hasDiffuseMap)
		return texture(material.diffuseMap, varying_textureCord).xyz;
	else
		return material.defaultDiffuseColor;
};

float CalculateAttenuation(int i)
{
	float distanceToLight = distance(lights[i].position, varying_position);//find distance between light position and surface position
	return 1.0 - smoothstep(0, lights[i].range, distanceToLight);//use hermite interpolation to get a strength of light basssed on distance
}



float CalculateDiffuse(vec3 dirToLight, vec3 normal)
{
	return max(dot(dirToLight, normal), 0.0);//calculate a positive angle between a vector and surface normal to find access to light
}



float CalculateSpecular(int i, vec3 normal)
{
	vec3 dirToCamera = normalize(cameraPosition - varying_position);//Surface -> Eye
	vec3 dirToLight = normalize(lights[i].position - varying_position);//Surface -> Light

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

	vec3 dirToLight = normalize(lights[i].position - varying_position);//get vector from surface to light
	float attenuation = CalculateAttenuation(i);//calculate light fall off

	vec3 ambient = fragmentColor * material.ambientIntensity;

	vec3 diffuse = fragmentColor * CalculateDiffuse(dirToLight, normal) * attenuation;//calculate light reaching surface and apply fall off
	
	vec3 specular =  specularColor * material.specularIntensity * CalculateSpecular(i, normal) * attenuation;//calculate specular on surface and apply fall off

	return (ambient + diffuse + specular) * lightColor;//final calculation
};

vec3 CalculateSpotLight(int i, vec3 normal)
{
	vec3 lightColor = lights[i].color * lights[i].intensity;//overall light color
	vec3 fragmentColor = GetFragmentDiffuse();
	vec3 specularColor = GetFragmentSpecular();

	vec3 dirToLight = normalize(lights[i].position - varying_position);//vector from surface to light
	float angleBetweenSpotDirectionAndCutOff = dot(-dirToLight, normalize(lights[i].direction));//calculate angle of fragment from light direction
	
	vec3 ambient = fragmentColor * material.ambientIntensity;

	if(angleBetweenSpotDirectionAndCutOff > lights[i].cutOffAngle)//if the angle of the fragment is inside of the cutoff (radius/cone area) 
	{
		float attenuation = CalculateAttenuation(i);//calculte light fall off

		vec3 diffuse = fragmentColor * CalculateDiffuse(dirToLight, normal) * attenuation;

		vec3 specular = specularColor * material.specularIntensity * CalculateSpecular(i, normal) * attenuation;//calculate reachable specular and apply fall off

		//final calculation
		return (ambient + diffuse + specular) * lightColor;
	}
	return ambient * lightColor;

};
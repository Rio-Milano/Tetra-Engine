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
out vec4 fragColor;





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





/*
MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO MACRO
*/
#define NUMBER_OF_LIGHTS 10





/*
 UNIFORM UNIFORM UNIFORM UNIFORM UNIFORM UNIFORM
*/
uniform sampler2D textureSampler;
uniform Light lights [NUMBER_OF_LIGHTS];
uniform vec3 cameraPosition;
uniform float ambientIntensity;

/*
FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS 
*/
float CalculateAttenuation(int i);
float CalculateDiffuse(vec3 dirToLight, vec3 normal);
float CalculateSpecular(int i, vec3 normal);

vec3 CalculateDirectionalLight(int i, vec3 normal);
vec3 CalculatePointLight(int i, vec3 normal);
vec3 CalculateSpotLight(int i, vec3 normal);





void main()
{
	vec3 normal = normalize(varying_normal);//check on why normal must be renormalized... scaling in vertex shader??
	vec3 totalRecivedLight;

	for(int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{

		if(!lights[i].inUse)
			continue;

		switch(lights[i].type)
		{
			case 0U://DIRECTIONAL
				totalRecivedLight += CalculateDirectionalLight(i, normal);
				break;

			case 1U://POINT
				totalRecivedLight += CalculatePointLight(i, normal);
				break;

			case 2U://SPOT
				totalRecivedLight  += CalculateSpotLight(i, normal);
				break;
			default:
				break;
		};

	};

	vec4 textureColor = texture(textureSampler, varying_textureCord);
	fragColor = vec4(totalRecivedLight, 1.0) * textureColor;

}

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

	float shinyness = 30.0;//would come from a specular map 

	return pow(CalculateDiffuse(dirToCamera, reflected), shinyness);//specular calculation
}



vec3 CalculateDirectionalLight(int i, vec3 normal)
{
	vec3 lightColor = lights[i].color * lights[i].intensity; // calculate overall light color
	vec3 inverseLightDirection = normalize(-lights[i].direction);//get vector pointing towards directional light
	
	float diffuse = CalculateDiffuse(inverseLightDirection, normal);//find angle netween surface normal and inverse directional light
	float specular = CalculateSpecular(i, normal);//calculate specular

	return (ambientIntensity + diffuse + specular) * lightColor;//calculate the final light color
};



vec3 CalculatePointLight(int i, vec3 normal)
{
	vec3 lightColor = lights[i].color * lights[i].intensity;//calculate overall light color
	vec3 dirToLight = normalize(lights[i].position - varying_position);//get vector from surface to light
	
	float attenuation = CalculateAttenuation(i);//calculate light fall off
	float diffuse = CalculateDiffuse(dirToLight, normal) * attenuation;//calculate light reaching surface and apply fall off
	float specular = CalculateSpecular(i, normal) * attenuation;//calculate specular on surface and apply fall off

	return (ambientIntensity + diffuse + specular) * lightColor;//final calculation
};

vec3 CalculateSpotLight(int i, vec3 normal)
{
	vec3 lightColor = lights[i].color * lights[i].intensity;//overall light color

	vec3 dirToLight = normalize(lights[i].position - varying_position);//vector from surface to light
	
	float angleBetweenSpotDirectionAndCutOff = dot(-dirToLight, normalize(lights[i].direction));//calculate angle of fragment from light direction
	
	if(angleBetweenSpotDirectionAndCutOff > lights[i].cutOffAngle)//if the angle of the fragment is inside of the cutoff (radius/cone area) 
	{
		float attenuation = CalculateAttenuation(i);//calculte light fall off

		float diffuse = CalculateDiffuse(dirToLight, normal) * attenuation;//calculate reachable light and apply fall off
		float specular = CalculateSpecular(i, normal)* attenuation;//calculate reachable specular and apply fall off

		//final calculation
		return (ambientIntensity + diffuse + specular) * lightColor;
	}
	return vec3(0.0);

};
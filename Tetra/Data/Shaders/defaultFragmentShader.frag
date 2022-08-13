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
	
	float intensity;
	
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


/*
FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS FUNCTIONS 
*/
vec3 CalculateDirectionalLight(int i, vec3 normal)
{
	vec3 lightColor = lights[i].color * lights[i].intensity;

	vec3 inverseLightDirection = normalize(-lights[i].direction);
	float angularIntensity = max(dot(inverseLightDirection, normal), 0.0);

	return lightColor * angularIntensity;
};
//
vec3 CalculateAmbientLight(int i)
{
	return lights[i].color * lights[i].intensity;
};
//
vec3 CalculatePointLight(int i, vec3 normal)
{
	vec3 lightColor = lights[i].color * lights[i].intensity;
	
	vec3 dirToLight = normalize(lights[i].position - varying_position);
	float angularIntensity = max(dot(dirToLight, normal), 0.0);

	return lightColor * angularIntensity;
};
//
vec3 CalculateSpotLight(int i)
{
	return vec3(0);
};





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
			case 0U://AMBIENT
				totalRecivedLight += CalculateAmbientLight(i);
				break;

			case 1U://DIRECTIONAL
				totalRecivedLight += CalculateDirectionalLight(i, normal);
				break;

			case 2U://POINT
				totalRecivedLight += CalculatePointLight(i, normal);
				break;

			case 3U://SPOT
				totalRecivedLight += CalculateSpotLight(i);
				break;
			default:
				break;
		};

	};

	vec4 textureColor = texture(textureSampler, varying_textureCord);
	fragColor = vec4(totalRecivedLight, 1.0) * textureColor;

}
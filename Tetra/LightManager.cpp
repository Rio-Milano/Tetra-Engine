#include "LightManager.h"

#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()


void LightManager::Initialize()
{
	//resize vector of lights
	m_lights.resize(NUMBER_OF_LIGHTS);

	//define custom vertz for the light mesh
	std::vector<glm::vec3> positions =
	{
		glm::vec3(-0.5f,0.5f,-0.5f),
		glm::vec3(-0.5f,0.5f,0.5f),
		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,0.5f,0.5f),

		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,0.5f)
	};

	std::vector<unsigned int> elements
	{
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7,
		0,1,5,
		0,4,5,
		2,3,7,
		2,6,7,
		0,2,6,
		0,4,6,
		1,5,7,
		1,3,7
	};

	//create a shader to draw the lights
	Shader cubeShader;
	cubeShader.Create("Data/Shaders/lightCubeVertexShader.vert", "Data/Shaders/lightCubeFragmentShader.frag");
	//add the shader to storage
	ShaderManager.AddShader("lightCubeShader", cubeShader);

	//generate the mesh used for drawing the lights
	m_meshForLight.GenerateMesh(positions, {}, {}, elements, 0, GL_STATIC_DRAW, "lightCubeShader");

}

void LightManager::SetShader(Shader* shader)
{
	//set the shader to recieve lighting state and aksi
	m_shader = shader;
}

void LightManager::SetDirectionalLight(const glm::vec3& direction, const glm::vec3& color, const float& intensity, int index)
{
	//if no index set
	if (index == -1)
	{
		//get new index
		int freeLightIndex = GetFreeLight();

		//new index is unavaliable
		if (freeLightIndex == -1)
		{
			std::cout << "No lights avaliable!\n";
			return;
		}
		//set new index
		index = freeLightIndex;
	}
	//get light at index
	Light& light = m_lights[index];

	//assign values of light
	light.m_inUse = true;
	light.m_lightColor = color;
	light.m_direction = direction;
	light.m_lightIntensity = intensity;
	light.m_lightType = LightType::Directional;
}


void LightManager::SetSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, const float& intensity, int index, float innerCutOff, float outerCutoff, const float& range)
{
	//if no index set
	if (index == -1)
	{
		//get new index
		int freeLightIndex = GetFreeLight();

		//new index is unavaliable
		if (freeLightIndex == -1)
		{
			std::cout << "No lights avaliable!\n";
			return;
		}
		//set new index
		index = freeLightIndex;
	}

	//get light from index
	Light& light = m_lights[index];
	//assign values of light
	light.m_inUse = true;
	light.m_lightColor = color;
	light.m_direction = direction;
	light.m_lightIntensity = intensity;
	light.m_range = range;
	light.m_position = position;
	light.m_innerCutOffAngle = innerCutOff;
	light.m_outerCutOffAngle = outerCutoff;
	light.m_lightType = LightType::Spot;
}

void LightManager::SetPointLight(const glm::vec3& position, const glm::vec3& color, const float& intensity, const float& range, int index)
{
	//if no index set
	if (index == -1)
	{
		//get new index
		int freeLightIndex = GetFreeLight();

		//new index is unavaliable
		if (freeLightIndex == -1)
		{
			std::cout << "No lights avaliable!\n";
			return;
		}
		//set new index
		index = freeLightIndex;
	}

	//get light at index
	Light& light = m_lights[index];

	//set light attributes
	light.m_inUse = true;
	light.m_position = position;
	light.m_lightColor = color;
	light.m_lightIntensity = intensity;
	light.m_range = range;
	light.m_lightType = LightType::Point;
}

Light& LightManager::GetLight(const int& index)
{
	//throw break point if index out of range
	_ASSERT(index < NUMBER_OF_LIGHTS && index >= 0);
	
	return m_lights[index];
}

void LightManager::UpdateShader(const float& dt)
{
	//throw break pont if shader is nullptr
	_ASSERT(m_shader);

	//loop lights
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		//set prefix used for getting location of light attributes from shader
		std::string prefix = "lights[" + std::to_string(i) + "].";

		//get each light attribute location
		GLuint lightPosLoc = m_shader->GetLocation(prefix + "position");
		GLuint innerLightCutOffAngleLoc = m_shader->GetLocation(prefix + "innerCutOffAngle");
		GLuint outerLightCutoffAngleLoc = m_shader->GetLocation(prefix + "outerCutOffAngle");
		GLuint lightColorLoc = m_shader->GetLocation(prefix + "color");
		GLuint lightIntensityLoc = m_shader->GetLocation(prefix + "intensity");
		GLuint lightTypeLoc = m_shader->GetLocation(prefix + "type");
		GLuint lightInUseLoc = m_shader->GetLocation(prefix + "inUse");
		GLuint lightDirLoc = m_shader->GetLocation(prefix + "direction");
		GLuint lightRangeLoc = m_shader->GetLocation(prefix + "range");

		//get current light
		Light& light = m_lights[i];

		//set each light attribute location in shader to light attribute in memory
		m_shader->SetUniform3fv(lightColorLoc, light.m_lightColor);
		m_shader->SetUniform1f(lightIntensityLoc, light.m_lightIntensity);
		m_shader->SetUniform1b(lightInUseLoc, light.m_inUse);
		m_shader->SetUniform1i(lightTypeLoc, static_cast<int>(light.m_lightType));
		m_shader->SetUniform3fv(lightDirLoc, light.m_direction);
		m_shader->SetUniform3fv(lightPosLoc, light.m_position);
		m_shader->SetUniform1f(lightRangeLoc, light.m_range);
		m_shader->SetUniform1f(innerLightCutOffAngleLoc, static_cast<float>(cos(glm::radians(light.m_innerCutOffAngle))));
		m_shader->SetUniform1f(outerLightCutoffAngleLoc, static_cast<float>(cos(glm::radians(light.m_outerCutOffAngle))));

		//if light is a spot light
		if (light.m_lightType == LightType::Spot)
		{
			//if spot light is dynamic
			if (light.simulateLight)
			{
				//have it rotate arround the origin using a set angle
				float x = cos(glm::radians(light.lightAngle)) * light.radius;
				float z = sin(glm::radians(light.lightAngle)) * light.radius;
				light.m_position = glm::vec3(x, light.m_position.y, z);

				//make angle bigger
				light.lightAngle += light.rotationSpeed * dt;
				if (light.lightAngle > 360.f)
					light.lightAngle = 0.f;

				//set light to allways face origin
				light.m_direction = glm::normalize(glm::vec3(0.0f) - light.m_position);
			}
		}
	}

}

void LightManager::DrawLights(Renderer& renderer)
{
	//get the shader used to draw the lights
	Shader& shader = ShaderManager.GetShader("lightCubeShader");

	//loop lights
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		//get current light
		Light& light = m_lights[i];

		//if light is on
		if (light.m_inUse && light.m_drawLight)
		{
			//if light a point or spot light
			if (light.m_lightType != LightType::Directional)
			{
				//translate to light position
				glm::mat4 transform(1.0f);
				transform = glm::translate(transform, m_lights[i].m_position);
				
				//if light is spot
				if (light.m_lightType == LightType::Spot)
				{
					//compute a rotation matrix used to align vectors
					// 
					// this will rotate the light direction at the origin to the light direction in memory
					
					//code taken from :https://iquilezles.org/articles/noacos/
					const glm::vec3   axi = glm::normalize(glm::cross(light.m_direction, glm::vec3(-1, 0, 0)));
					const float  ang = acosf(glm::clamp(glm::dot(light.m_direction, glm::vec3(-1, 0, 0)), -1.0f, 1.0f));
					const float  co = cosf(ang);
					const float  si = sinf(ang);
					const float  ic = 1.0f - co;
					//calculate a rotation matrix to rotate vector A to B
					const glm::mat4 rot =
					{ 
						axi.x * axi.x * ic + co,			axi.y * axi.x * ic - si * axi.z,	axi.z * axi.x * ic + si * axi.y,	0,
						axi.x * axi.y * ic + si * axi.z,	axi.y * axi.y * ic + co,			axi.z * axi.y * ic - si * axi.x,	0,
						axi.x * axi.z * ic - si * axi.y,	axi.y * axi.z * ic + si * axi.x,	axi.z * axi.z * ic + co,			0,
						0,									0,									0,									1 
					};
					transform *= rot;//align the mesh direction with light direction

					glm::vec3 spotLightScale(1.f, 0.3f, 0.3f);
					transform = glm::scale(transform, spotLightScale);//turn the mesh into an oblong

					glm::vec3 shiftTransform(-0.5f, 0.0f, 0.0f);
					transform = glm::translate(transform, shiftTransform);//have the mesh start at 0 and finish along the -x axis

					//this will be the smaller part of the model of the spot light that looks like a bulb
					
					glm::mat4 transform_2(1.0f);
					transform_2 = glm::translate(transform_2, light.m_position);//translate mesh to light position
					transform_2 *= rot;//aligh it with direction of light
					transform_2 = glm::scale(transform_2, glm::vec3(0.6f, 0.6f, 0.6f));//scale down mesh
					
					glm::vec3 overallLightColor = light.m_lightColor * light.m_lightIntensity;
					shader.SetUniform3fv(shader.GetLocation("cubeColor"), overallLightColor);//set bulb color to light color
					//render the bulb
					renderer.RenderMesh(m_meshForLight, transform_2);

				}
				else
				{
					//point light scaling
					transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
				}
				//calculate light color for light holder mesh
				glm::vec3 overallLightColor = light.m_lightColor * light.m_lightIntensity;
				//set bulb holder color
				shader.SetUniform3fv(shader.GetLocation("cubeColor"), overallLightColor);
				//render bulb holder
				renderer.RenderMesh(m_meshForLight, transform);
				
			}
		}
	}
}

int LightManager::GetFreeLight()
{
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		if (!m_lights[i].m_inUse)
			return i;
	}
	return -1;
};

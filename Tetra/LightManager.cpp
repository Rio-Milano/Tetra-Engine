#include "LightManager.h"

void LightManager::Initialize()
{
	m_lights.resize(NUMBER_OF_LIGHTS);

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

	Shader cubeShader;
	cubeShader.Create("Data/Shaders/lightCubeVertexShader.vert", "Data/Shaders/lightCubeFragmentShader.frag");
	ShaderManager.AddShader("lightCubeShader", cubeShader);

	m_meshForLight.GenerateMesh(positions, "", "", "", 0, elements);
	m_meshForLight.SetProgramName("lightCubeShader");
}

void LightManager::SetShaderID(Shader* shader)
{
	m_shader = shader;
	GLuint ambientIntensityLoc = m_shader->GetLocation("ambientIntensity");
	m_shader->SetUniform1f(ambientIntensityLoc, 0.05f);
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

		index = freeLightIndex;
	}

	Light& light = m_lights[index];

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

		index = freeLightIndex;
	}

	Light& light = m_lights[index];

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

		index = freeLightIndex;
	}

	Light& light = m_lights[index];

	light.m_inUse = true;
	light.m_position = position;
	light.m_lightColor = color;
	light.m_lightIntensity = intensity;
	light.m_range = range;
	light.m_lightType = LightType::Point;
}

Light& LightManager::GetLight(const int& index)
{
	static Light instance;
	if (index >= NUMBER_OF_LIGHTS)
	{
		std::cout << "Light index is invalid!\n";
		return instance;
	}
	return m_lights[index];
}

void LightManager::UpdateShader()
{
	if (!m_shader)
	{
		std::cout << "Light Manager failed to use shader!\n";
		return;
	}

	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		std::string prefix = "lights[" + std::to_string(i) + "].";

		GLuint lightPosLoc = m_shader->GetLocation(prefix + "position");
		GLuint innerLightCutOffAngleLoc = m_shader->GetLocation(prefix + "innerCutOffAngle");
		GLuint outerLightCutoffAngleLoc = m_shader->GetLocation(prefix + "outerCutOffAngle");
		GLuint lightColorLoc = m_shader->GetLocation(prefix + "color");
		GLuint lightIntensityLoc = m_shader->GetLocation(prefix + "intensity");
		GLuint lightTypeLoc = m_shader->GetLocation(prefix + "type");
		GLuint lightInUseLoc = m_shader->GetLocation(prefix + "inUse");
		GLuint lightDirLoc = m_shader->GetLocation(prefix + "direction");
		GLuint lightRangeLoc = m_shader->GetLocation(prefix + "range");

		Light& light = m_lights[i];

		m_shader->SetUniform3fv(lightColorLoc, light.m_lightColor);
		m_shader->SetUniform1f(lightIntensityLoc, light.m_lightIntensity);
		m_shader->SetUniform1b(lightInUseLoc, light.m_inUse);
		m_shader->SetUniform1i(lightTypeLoc, static_cast<int>(light.m_lightType));
		m_shader->SetUniform3fv(lightDirLoc, light.m_direction);
		m_shader->SetUniform3fv(lightPosLoc, light.m_position);
		m_shader->SetUniform1f(lightRangeLoc, light.m_range);
		m_shader->SetUniform1f(innerLightCutOffAngleLoc, static_cast<float>(cos(glm::radians(light.m_innerCutOffAngle))));
		m_shader->SetUniform1f(outerLightCutoffAngleLoc, static_cast<float>(cos(glm::radians(light.m_outerCutOffAngle))));

	}

}

void LightManager::DrawLights(Renderer& renderer)
{
	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		if (m_lights[i].m_inUse)
		{
			if (m_lights[i].m_lightType != LightType::Directional)
			{
				Shader& shader = ShaderManager.GetShader("lightCubeShader");

				glm::mat4 transform(1.0f);
				transform = glm::translate(transform, m_lights[i].m_position);
				transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

				shader.SetUniformMat4f(shader.GetLocation("world"), transform);

				glm::vec3 overallLightColor = m_lights[i].m_lightColor * m_lights[i].m_lightIntensity;

				shader.SetUniform3fv(shader.GetLocation("cubeColor"), overallLightColor);

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

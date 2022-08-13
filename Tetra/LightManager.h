#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#define NUMBER_OF_LIGHTS 10

#include<vector>
#include<glad/glad.h>
#include<iostream>
#include<string>

#include"Shader.h"
#include"glmIncludes.h"
#include"Mesh.h"
#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()
#include"Renderer.h"

enum class LightType
{
	Ambient = 0,
	Directional,
	Point,
	Spot

};

struct Light
{
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_lightColor;
	float m_lightIntensity;
	LightType m_lightType;
	bool m_inUse;
};

class LightManager
{
public:

	LightManager() = default;

	void Initialize()
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


		m_meshForLight.GenerateMesh(positions, "", 0, elements);
		m_meshForLight.SetProgramID(cubeShader.GetID());
	}

	void SetShaderID(Shader* shader)
	{
		m_shader = shader;
	}


	void SetAmbientLight(const glm::vec3& color, const float& intensity)
	{
		int freeLightIndex = GetFreeLight();
		
		if (freeLightIndex == -1)
			std::cout << "No lights avaliable!\n";

		Light& light = m_lights[freeLightIndex];

		light.m_inUse = true;
		light.m_lightColor = color;
		light.m_lightIntensity = intensity;
		light.m_lightType = LightType::Ambient;

	};

	void SetDirectionalLight(const glm::vec3& direction, const glm::vec3& color, const float& intensity)
	{
		int freeLightIndex = GetFreeLight();

		if (freeLightIndex == -1)
			std::cout << "No lights avaliable!\n";

		Light& light = m_lights[freeLightIndex];

		light.m_inUse = true;
		light.m_lightColor = color;
		light.m_direction = direction;
		light.m_lightIntensity = intensity;
		light.m_lightType = LightType::Directional;

	};

	void SetPointLight(const glm::vec3& position, const glm::vec3& color, const float& intensity)
	{
		int freeLightIndex = GetFreeLight();

		if (freeLightIndex == -1)
			std::cout << "No lights avaliable!\n";

		Light& light = m_lights[freeLightIndex];

		light.m_inUse = true;
		light.m_position = position;
		light.m_lightColor = color;
		light.m_lightIntensity = intensity;
		light.m_lightType = LightType::Point;
	}


	void UpdateShader()
	{
		if (!m_shader)
		{
			std::cout << "Light Manager failed to use shader!\n";
			return;
		}

		GLuint shaderID = m_shader->GetID();

		for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
		{
			std::string prefix = "lights[" + std::to_string(i) + "].";
			
			GLuint lightPosLoc =  m_shader->GetLocation(prefix + "position");
			GLuint lightColorLoc = m_shader->GetLocation(prefix + "color");
			GLuint lightIntensityLoc = m_shader->GetLocation(prefix + "intensity");
			GLuint lightTypeLoc = m_shader->GetLocation(prefix + "type");
			GLuint lightInUseLoc = m_shader->GetLocation(prefix + "inUse");
			GLuint lightDirLoc = m_shader->GetLocation(prefix + "direction");

			Light& light = m_lights[i];

			m_shader->SetUniform3fv(lightColorLoc, &light.m_lightColor.x);
			m_shader->SetUniform1f(lightIntensityLoc, light.m_lightIntensity);
			m_shader->SetUniform1b(lightInUseLoc, light.m_inUse);
			m_shader->SetUniform1ui(lightTypeLoc, static_cast<unsigned int>(light.m_lightType));
			m_shader->SetUniform3fv(lightDirLoc, &light.m_direction.x);
			m_shader->SetUniform3fv(lightPosLoc, &light.m_position.x);

			//switch (light.m_lightType)
			//{
			//	case LightType::Ambient:
			//		break;

			//	case LightType::Directional:
			//		break;

			//	case LightType::Point:
			//		break;

			//	case LightType::Spot:
			//		break;

			//	default:
			//		std::cout << "LightType not recognised!\n;";
			//		break;
			//}

		}
	}

	void DrawLights(Renderer& renderer)
	{
		for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
		{
			if (m_lights[i].m_inUse)
			{
				if (m_lights[i].m_lightType == LightType::Point)
				{
					Shader& shader = ShaderManager.GetShader("lightCubeShader");

					glm::mat4 transform(1.0f);
					transform = glm::translate(transform, m_lights[i].m_position);
					transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

					shader.SetUniformMat4f(shader.GetLocation("world"), transform);

					shader.SetUniform3fv(shader.GetLocation("cubeColor"), &m_lights[i].m_lightColor.x);

					renderer.RenderMesh(m_meshForLight);
				}
			}
		}
	}


private:
	int GetFreeLight()
	{
		for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
		{
			if (!m_lights[i].m_inUse)
				return i;
		}
		return -1;
	};

	Mesh m_meshForLight;
	Shader* m_shader{nullptr};
	std::vector<Light> m_lights;
};

#endif
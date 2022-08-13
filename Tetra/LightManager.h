#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#define NUMBER_OF_LIGHTS 10

#include<vector>
#include<glad/glad.h>
#include<iostream>
#include<string>

#include"Shader.h"
#include"glmIncludes.h"

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

	LightManager()
	{
		m_lights.resize(NUMBER_OF_LIGHTS);
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
			
			GLuint lightPosLoc = glGetUniformLocation(shaderID, std::string(prefix + "position").c_str());
			GLuint lightColorLoc = glGetUniformLocation(shaderID, std::string(prefix + "color").c_str());
			GLuint lightIntensityLoc = glGetUniformLocation(shaderID, std::string(prefix + "intensity").c_str());
			GLuint lightTypeLoc = glGetUniformLocation(shaderID, std::string(prefix + "type").c_str());
			GLuint lightInUseLoc = glGetUniformLocation(shaderID, std::string(prefix + "inUse").c_str());
			GLuint lightDirLoc = glGetUniformLocation(shaderID, std::string(prefix + "direction").c_str());


			Light& light = m_lights[i];

			switch (light.m_lightType)
			{
				case LightType::Ambient:
					m_shader->SetUniform3fv(lightColorLoc, &light.m_lightColor.x);
					m_shader->SetUniform1f(lightIntensityLoc, light.m_lightIntensity);
					m_shader->SetUniform1b(lightInUseLoc, light.m_inUse);
					m_shader->SetUniform1ui(lightTypeLoc, static_cast<unsigned int>(light.m_lightType));

					break;

				case LightType::Directional:
					m_shader->SetUniform3fv(lightColorLoc, &light.m_lightColor.x);
					m_shader->SetUniform1f(lightIntensityLoc, light.m_lightIntensity);
					m_shader->SetUniform1b(lightInUseLoc, light.m_inUse);
					m_shader->SetUniform3fv(lightDirLoc, &light.m_direction.x);
					m_shader->SetUniform1ui(lightTypeLoc, static_cast<unsigned int>(light.m_lightType));
					break;

				case LightType::Point:
					m_shader->SetUniform3fv(lightPosLoc, &light.m_position.x);
					m_shader->SetUniform3fv(lightColorLoc, &light.m_lightColor.x);
					m_shader->SetUniform1f(lightIntensityLoc, light.m_lightIntensity);
					m_shader->SetUniform1b(lightInUseLoc, light.m_inUse);
					m_shader->SetUniform1ui(lightTypeLoc, static_cast<unsigned int>(light.m_lightType));
					break;

				case LightType::Spot:
					break;

				default:
					std::cout << "LightType not recognised!\n;";
					break;
			}

			/*
			
			m_shader->SetUniform3fv(lightPosLoc, &light.m_position.x);

			m_shader->SetUniform3fv(lightColorLoc, &light.m_lightColor.x);

			m_shader->SetUniform1f(lightIntensityLoc, light.m_lightIntensity);

			m_shader->SetUniform1ui(lightTypeLoc, static_cast<unsigned int>(light.m_lightType));

			m_shader->SetUniform1b(lightInUseLoc, light.m_inUse);

			*/

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

	Shader* m_shader{nullptr};
	std::vector<Light> m_lights;
};

#endif
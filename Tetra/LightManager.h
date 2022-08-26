#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#define NUMBER_OF_LIGHTS 10
#define MAX_RANGE 100.f
#define MIN_XYZ -50.f
#define MAX_XYZ 50.f
#define MIN_COLOR 0.f
#define MAX_COLOR 1.f
#define MIN_DIRECTION_XYZ -1.f
#define MAX_DIRECTION_XYZ 1.f
#define MIN_CONE_ANGLE 0.f
#define MAX_CONE_ANGLE 90.f

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
	Directional=0,
	Point,
	Spot

};

struct Light
{
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_lightColor;
	float m_lightIntensity;
	float m_innerCutOffAngle;
	float m_outerCutOffAngle;
	LightType m_lightType;
	bool m_inUse;
	float m_range;

	bool simulateLight = false;
		float lightAngle = 0.0f;
		float radius = 10.f;
		float rotationSpeed = 2.0f;

};

class LightManager
{
public:

	LightManager() = default;

	void Initialize();

	void SetShaderID(Shader* shader);


	
	void SetDirectionalLight(const glm::vec3& direction, const glm::vec3& color, const float& intensity, int index = -1);

	
	void SetSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, const float& intensity, int index = -1, float innerCutOff = 6.0f, float outerCutoff = 12.5f, const float& range = 40.f);

	void SetPointLight(const glm::vec3& position, const glm::vec3& color, const float& intensity, const float& range = { 40.f }, int index = -1);

	Light& GetLight(const int& index);

	void UpdateShader(const float& dt);

	void DrawLights(Renderer& renderer);


private:
	int GetFreeLight();

	Mesh m_meshForLight;
	Shader* m_shader{nullptr};
	std::vector<Light> m_lights;
};

#endif
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

#include"../../OpenGL_Abstraction/H/Shader.h"
#include"../../Requirements/glmIncludes.h"
#include"../../Structure/H/Mesh.h"
#include"../../Graphics/H/Renderer.h"

class BaseLayer;

//types of light
enum class LightType
{
	Directional=0,
	Point,
	Spot
};

//deffinition of a light
struct Light
{
	glm::vec3 m_position{};
	glm::vec3 m_direction{};
	glm::vec3 m_lightColor{};
	float m_lightIntensity{};
	float m_innerCutOffAngle{};
	float m_outerCutOffAngle{};
	LightType m_lightType{};
	bool m_inUse{};
	float m_range{};

	bool simulateLight{ false };
	float lightAngle{ 0.0f };
		float radius{ 10.0f };
		float rotationSpeed{ 2.0f };

	bool m_drawLight{true};
};

class LightManager
{
public:
	//ctors and dtors
	LightManager() = default;
	~LightManager();

	//create a mesh for light and make light shader
	void Initialize();

	//set the shader to recieve the realtime state of the lights
	void SetShader(Shader* shader);

	//configure the lights
	void SetDirectionalLight(const glm::vec3& direction, const glm::vec3& color, const float& intensity, int index = -1);
	void SetSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, const float& intensity, int index = -1, float innerCutOff = 6.0f, float outerCutoff = 12.5f, const float& range = 40.f);
	void SetPointLight(const glm::vec3& position, const glm::vec3& color, const float& intensity, const float& range = { 40.f }, int index = -1);

	//get a spessfic light by index
	Light& GetLight(const int& index);

	//send the lights state to the shader used for model rendering
	void UpdateShader(const float& dt);

	//draw physical representations of lights
	void DrawLights(Renderer& renderer);

	void DrawSceneToDepthBuffer(BaseLayer* baseLayer);

	const GLuint GetDepthMap() const;

private:
	//internal helper that gets a ligh not in use
	int GetFreeLight();
	void InitializeDepthFrameBuffer();

	//mesh that represents the lights
	Mesh m_meshForLight;
	//shader to recieve real time light states
	Shader* m_shader{nullptr};
	//vector of all lights
	std::vector<Light> m_lights;

	/*
	Shadow Mapping for directional lights
	*/
	GLuint m_depthFrameBuffer{0};
	GLuint m_depthMap{0};
};

#endif
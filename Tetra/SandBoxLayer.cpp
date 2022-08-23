#include "SandBoxLayer.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()
#include"InputManager.h"
#define InputManager InputManager::GetInstance()
#include "external_libaries/include/imGUI/imgui.h"

void SandBoxLayer::Start()
{
	Texture t1;
	t1.InitializeTexture("Data/Images/Box.png", ImageType::png);
	TextureManager.AddTexture("Box", t1);

	Texture t2;
	t2.InitializeTexture("Data/Images/BoxSpec.png", ImageType::png);
	TextureManager.AddTexture("BoxSpec", t2);

	Texture t3;
	t3.InitializeTexture("Data/Images/BoxEmission.png", ImageType::png);
	TextureManager.AddTexture("BoxEmission", t3);

	m_entity.Init();
	
	m_lightManager.Initialize();

	m_lightManager.SetShaderID(&ShaderManager.GetShader("main"));
	
	m_lightManager.SetDirectionalLight(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f), 0.2f);//0
	m_lightManager.SetPointLight(glm::vec3(-8.f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), .5f);//1
	m_lightManager.SetPointLight(glm::vec3(8.f, 0.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f), .5f);//2
	m_lightManager.SetPointLight(glm::vec3(0.f, 0.f, 8.f), glm::vec3(0.0f, 0.0f, 1.0f), .5f);//3
	m_lightManager.SetPointLight(glm::vec3(0.f, 0.f, -8.f), glm::vec3(0.0f, 1.0f, 0.0f), .5f);//4

}



void SandBoxLayer::Update(float dt)
{
	if (m_wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	m_entity.Update(dt);
	//m_lightManager.SetSpotLight(m_camera.GetPosition(), m_camera.GetForwardVector(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 5);
	

	/*Light& light = m_lightManager.GetLight(4);
	static float theta = 0;
	static const float vx = 1.f;

	if (glfwGetKey(m_renderer.GetWindow().GetWindowPtr(), GLFW_KEY_LEFT) == GLFW_PRESS)
		theta -= vx * dt;

	if (glfwGetKey(m_renderer.GetWindow().GetWindowPtr(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		theta += vx * dt;

	if (theta < -1.f)
		theta = -1.f;
	else if (theta > 1.f)
		theta = 1.f;

	light.m_direction.x = theta;*/

	m_lightManager.UpdateShader();
}

void SandBoxLayer::Render()
{

	m_lightManager.DrawLights(m_renderer);
	m_entity.Render(m_renderer);
}

void SandBoxLayer::End()
{
}

void SandBoxLayer::ImGUI()
{

	float buttonX = 140.f;
	float buttonY = 20.f;

	ImGui::Begin("Lights");

	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		if (ImGui::BeginCombo(std::string("Light :" + std::to_string(i+1)).c_str(), ""))
		{
			Light& light = m_lightManager.GetLight(i);

			ImGui::Checkbox("Light in use", &light.m_inUse);
		
			if (light.m_lightType == LightType::Directional)
			{
				ImGui::Text("Directional Light");

				ImGui::SliderFloat3("Direction", &light.m_direction.x, MIN_DIRECTION_XYZ, MAX_DIRECTION_XYZ);
				ImGui::SliderFloat3("Color", &light.m_lightColor.x, MIN_COLOR, MAX_COLOR);
				ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
			}
			else if (light.m_lightType == LightType::Point)
			{
				ImGui::Text("Point Light");

				ImGui::SliderFloat3("Position", &light.m_position.x, MIN_XYZ, MAX_XYZ);
				ImGui::SliderFloat3("Color", &light.m_lightColor.x, MIN_COLOR, MAX_COLOR);
				ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
				ImGui::SliderFloat("Light range", &light.m_range, 0, MAX_RANGE);

			}
			else if (light.m_lightType == LightType::Spot)
			{
				ImGui::Text("Spot Light");
			
				ImGui::SliderFloat3("Position", &light.m_position.x, MIN_XYZ, MAX_XYZ);
				ImGui::SliderFloat3("Direction", &light.m_direction.x, MIN_DIRECTION_XYZ, MAX_DIRECTION_XYZ);
				ImGui::SliderFloat3("Color", &light.m_lightColor.x, MIN_COLOR, MAX_COLOR);
				ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
				ImGui::SliderFloat("Inner Cutoff Angle", &light.m_innerCutOffAngle, MIN_CONE_ANGLE, MAX_CONE_ANGLE);
				ImGui::SliderFloat("Outer Cutoff Angle", &light.m_outerCutOffAngle, MIN_CONE_ANGLE, MAX_CONE_ANGLE);
				ImGui::SliderFloat("Light range", &light.m_range, 0, MAX_RANGE);

			}

			ImGui::Text("Select Light Type");

			ImGui::SameLine();
			if (ImGui::Button("Directional Light", ImVec2(buttonX, buttonY)))
				light.m_lightType = LightType::Directional;

			ImGui::SameLine();
			if (ImGui::Button("Point Light", ImVec2(buttonX, buttonY)))
				light.m_lightType = LightType::Point;

			ImGui::SameLine();
			if (ImGui::Button("Spot Light", ImVec2(buttonX, buttonY)))
				light.m_lightType = LightType::Spot;

			

			ImGui::EndCombo();
		}

	}

	

	ImGui::End();

}
#include "../H/SandBoxLayer.h"
#include"../../Managers/H/TextureManager.h"
#define TextureManager TextureManager::GetInstance()
#include"../../Managers/H/InputManager.h"
#define InputManager InputManager::GetInstance()
#include "../../external_libaries/include/imGUI/imgui.h"
#include"../../Managers/H/MeshManager.h"
#define MeshManager MeshManager::GetInstance()
#include"../../Graphics/H/PostProcessing.h"

#include"../../Entities/H/AquaPig.h"
#include"../../Entities/H/Container.h"
#include"../../Entities/H/Line.h"
#include"../../Entities/H/BaseGrid.h"
#include"../../Entities/H/Plane.h"
#include"../../Entities/H/Grass.h"
#include"../../Entities/H/TransparentEntity.h"
#include"../../Entities/H/SkyBox.h"
#include"../../Entities/H/Backpack.h"

#include"../../OpenGL_Abstraction/H/Framebuffer.h"

void SandBoxLayer::Start()
{
	//add entities to entity vector
	m_entities.emplace_back(std::make_shared<AquaPig>("Boat"));
	m_entities.emplace_back(std::make_shared<Container>("Box"));
	
	m_entities.emplace_back(std::make_shared<BaseGrid>());

	m_entities.emplace_back(std::make_shared<Plane>());

	m_entities.emplace_back(std::make_shared<BackPack>("BackPack"));

	m_entities.emplace_back(std::make_shared<Grass>());

	m_entities.emplace_back(std::make_shared<TransparentEntity>());

	//must come last for optimization
	std::shared_ptr<SkyBox> skyBox = std::make_shared<SkyBox>();
	skyBox->SetFaceLocations({
			"Data/Textures/SkyBox/City/posx.jpg",
			"Data/Textures/SkyBox/City/negx.jpg",
			"Data/Textures/SkyBox/City/posy.jpg",
			"Data/Textures/SkyBox/City/negy.jpg",
			"Data/Textures/SkyBox/City/posz.jpg",
			"Data/Textures/SkyBox/City/negz.jpg"
		});
	m_entities.emplace_back(skyBox);

	//init entities
	for (const std::shared_ptr<Entity>& entity : m_entities)
		entity->Init();

	m_lightManager.Initialize();
	m_lightManager.SetShader(&ShaderManager.GetShader("main"));
	
	m_lightManager.SetDirectionalLight(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f), 0.2f);//0
	m_lightManager.SetPointLight(glm::vec3(-8.f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), .5f);//1
	m_lightManager.SetPointLight(glm::vec3(8.f, 0.f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f), .5f);//2
	m_lightManager.SetPointLight(glm::vec3(0.f, 0.f, 8.f), glm::vec3(0.0f, 0.0f, 1.0f), .5f);//3
	m_lightManager.SetPointLight(glm::vec3(0.f, 0.f, -8.f), glm::vec3(0.0f, 1.0f, 0.0f), .5f);//4

	//using window size as its same as view port but if view port was smaller then would need to use viewport size not window size
	m_postProcessing = std::make_shared<PostProcessing>(m_renderer.GetWindow().GetWindowSize(), true);
}



void SandBoxLayer::Update(float dt)
{
	//only update if simulation is not paused
	if (!m_pauseSimulation)
	{
		Light& light = m_lightManager.GetLight(9);
		m_lightManager.SetSpotLight(m_camera.GetPosition(), m_camera.GetForwardVector(), light.m_lightColor, light.m_lightIntensity, 9, light.m_innerCutOffAngle, light.m_outerCutOffAngle, light.m_range);
		m_lightManager.GetLight(9).m_drawLight = false;

		//update 
		m_lightManager.UpdateShader(dt);//update real time light attribute changes on the gpu shader

		//loop entitys
		for (const std::shared_ptr<Entity>& entity : m_entities)
		{
			entity->Update(dt);
		}
	}
}

void SandBoxLayer::PreRender(Shader* overrideShader)
{
	m_lightManager.DrawSceneToDepthBuffer(this);

	if (!m_wireframeMode)
	{
		m_postProcessing->Render_To_Off_Screen_Buffer();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
	

}

void SandBoxLayer::PostRender()
{
	if (m_drawLightingAsMeshes) m_lightManager.DrawLights(m_renderer);
	if (!m_wireframeMode)m_postProcessing->Render_FrameBuffer(m_renderer);
}

void SandBoxLayer::Render(Shader* overideShader)
{
	//loop entities
	for (const std::shared_ptr<Entity>& entity : m_entities)
		entity->Render(m_renderer, overideShader);

	m_renderer.RenderTransparentMeshes(m_camera.GetPosition());
}

void SandBoxLayer::End()
{
}

void SandBoxLayer::ImGUI()
{
	static bool LightingGUIOpen = true;
	static bool SystemGUIOpen = true;

	//button sizes
	float buttonX = 140.f;
	float buttonY = 20.f;

	//Lighting GUI
	{

		if (LightingGUIOpen)
		{
			ImGui::Begin("Lights");//create window for light management

			if (ImGui::Button("Close", ImVec2(buttonX, buttonY)))//close window button
				LightingGUIOpen = false;

			for (int i = 0; i < NUMBER_OF_LIGHTS; i++)//loop lights
			{
				if (ImGui::BeginCombo(std::string("Light :" + std::to_string(i + 1)).c_str(), "", ImGuiComboFlags_HeightLargest))//create a drop down for each light
				{
					Light& light = m_lightManager.GetLight(i);//get current light

					ImGui::Checkbox("Light in use", &light.m_inUse);//is light being used in light model

					//each if 
					switch (light.m_lightType)
					{
					case LightType::Directional:
						ImGui::Text("Directional Light");

						ImGui::SliderFloat3("Direction", &light.m_direction.x, MIN_DIRECTION_XYZ, MAX_DIRECTION_XYZ);
						ImGui::ColorPicker3("Color", &light.m_lightColor[0]);
						ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
						break;
					
					case LightType::Point:
						ImGui::Text("Point Light");

						ImGui::SliderFloat3("Position", &light.m_position.x, MIN_XYZ, MAX_XYZ);
						ImGui::ColorPicker3("Color", &light.m_lightColor[0]);
						ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
						ImGui::SliderFloat("Light range", &light.m_range, 0, MAX_RANGE);
						break;

					case LightType::Spot:
						ImGui::Text("Spot Light");

						ImGui::SliderFloat3("Position", &light.m_position.x, MIN_XYZ, MAX_XYZ);
						ImGui::SliderFloat3("Direction", &light.m_direction.x, MIN_DIRECTION_XYZ, MAX_DIRECTION_XYZ);
						ImGui::ColorPicker3("Color", &light.m_lightColor[0]);
						ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
						ImGui::SliderFloat("Inner Cutoff Angle", &light.m_innerCutOffAngle, MIN_CONE_ANGLE, MAX_CONE_ANGLE);
						ImGui::SliderFloat("Outer Cutoff Angle", &light.m_outerCutOffAngle, MIN_CONE_ANGLE, MAX_CONE_ANGLE);
						ImGui::SliderFloat("Light range", &light.m_range, 0, MAX_RANGE);
						ImGui::Checkbox("Simulate Light", &light.simulateLight);
						if (light.simulateLight)//have light rotate about origin while also facing origin
						{
							ImGui::SliderFloat("Radius", &light.radius, 0.0f, 20.f);
							ImGui::SliderFloat("Rotation Speed", &light.rotationSpeed, 0.0f, 100.0f);
						}
						break;

					default:
						break;
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
	}

	
	//System GUI
	{
		if (SystemGUIOpen)
		{
			ImGui::Begin("Rendering");
			if (ImGui::Button("Close", ImVec2(buttonX, buttonY)))
				SystemGUIOpen = false;

			ImGui::Checkbox("Wireframe", &m_wireframeMode);
			ImGui::Checkbox("Pause Simulation", &m_pauseSimulation);
			ImGui::Checkbox("Draw Normals", &ModelNode::DrawNormals);
			PostProcessing::Config& ppConfig = m_postProcessing->m_config;
			

			ImGui::SliderFloat("X Sampling Offset Magnitude", &ppConfig.m_XsampleOffsetMagnitude, 1.0f, 1.0f / 1920.0f);
			ImGui::SliderFloat("Y Sampling Offset Magnitude", &ppConfig.m_YsampleOffsetMagnitude, 1.0f, 1.0f / 1080.0f);
			ImGui::SliderFloat("Gamma", &ppConfig.gamma, -5.2f, 5.2f);

			ImGui::Checkbox("Sharpen Kernel", &ppConfig.m_enableSharpeningKernel);
			ImGui::Checkbox("Blur Kernel", &ppConfig.m_enableBlurKernel);
			ImGui::Checkbox("Edge Highlight Kernel", &ppConfig.m_edgeDetectionKernel);
			ImGui::Checkbox("Weird Kernel", &ppConfig.m_enableWeirdKernel);
			ImGui::Checkbox("Invert Color", &ppConfig.m_enableColorBufferInversion);
			ImGui::Checkbox("Grey Scale Color", &ppConfig.m_enableGreyScaleColorBuffer);


			ImGui::End();
		}
	}

	///Control ImGui
	{
		ImGui::Begin("Controll");
		ImGui::Checkbox("Rendering", &SystemGUIOpen);
		ImGui::Checkbox("Lighting", &LightingGUIOpen);
		ImGui::Checkbox("Draw Lighting", &m_drawLightingAsMeshes);
		if (ImGui::Button("Exit"))
		{
			glfwSetWindowShouldClose(m_renderer.GetWindow().GetWindowPtr(), true);
		}
		ImGui::End();

	}
}
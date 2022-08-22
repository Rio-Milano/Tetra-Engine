#include "SandBoxLayer.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include "external_libaries/include/imGUI/imgui.h"

void SandBoxLayer::Start()
{
	Texture bricks;
	bricks.InitializeTexture("Data/Images/Container.jpg");
	TextureManager.AddTexture("Bricks", bricks);
	Texture bricks2;
	bricks2.InitializeTexture("Data/Images/Container_Spec.jpg");
	TextureManager.AddTexture("Bricks2", bricks2);

	m_entity.Init();
	
	m_lightManager.Initialize();

	m_lightManager.SetShaderID(&ShaderManager.GetShader("main"));
	
	m_lightManager.SetPointLight(glm::vec3(0.f, 0.f, 8.f), glm::vec3(1.f), .4f);//0
	m_lightManager.SetPointLight(glm::vec3(0.f, 0.f, -8.f), glm::vec3(1.f), .4f);//1
	m_lightManager.SetPointLight(glm::vec3(8.f, 0.f, 0.f), glm::vec3(1.f), .4f);//2
	m_lightManager.SetPointLight(glm::vec3(-8.f, 0.f, 0.f), glm::vec3(1.f), .4f);//3

	m_lightManager.SetDirectionalLight(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0);//4
}



void SandBoxLayer::Update(float dt)
{
	if (m_wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	m_entity.Update(dt);
	m_lightManager.SetSpotLight(m_camera.GetPosition(), m_camera.GetForwardVector(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 5);
	

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

	static float f = 0.0f;
	static int counter = 0;
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Checkbox("Wireframe", &m_wireframeMode);
	
	ImGui::End();


// 3. Show another simple window.
if (show_another_window)
{
	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Hello from another window!");
	if (ImGui::Button("Close Me"))
		show_another_window = false;
	ImGui::End();
}

}

#include "BaseLayer.h"
#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()
#include"InputManager.h"
#define InputManager InputManager::GetInstance()
#include "external_libaries/include/imGUI/imgui.h"
#include "external_libaries/include/imGUI/imgui_impl_glfw.h"
#include "external_libaries/include/imGUI/imgui_impl_opengl3.h"
#include"Helper.h"

#include<chrono>
#include<thread>

void BaseLayer::CreateLayer(const glm::vec<2, int> windowSize, const std::string& windowName)
{
	//create a window
	m_renderer.CreateWindowGLFW(windowSize.x, windowSize.y, windowName);
	//pull function pointers from gpu driver
	InitGLAD();
	//init the renderer
	m_renderer.InitRenderer();
	InputManager.InitializeInputManager(m_renderer.GetWindow().GetWindowPtr());
	//create default shaders
	CreateShader();
	//setup camera with attributes
	m_camera.Initialize(90.0f, static_cast<glm::vec2>(windowSize), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), 5.0f, .1f, glm::vec3(0.0f, 0.0f, 4.0f), m_renderer.GetWindow().GetWindowPtr());
	//output gl state
	Helper::Status::DisplayUsefulInfo();
	//init gui lib
	InitializeImGui();

}

void BaseLayer::InitializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_renderer.GetWindow().GetWindowPtr(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void BaseLayer::DestroyLayer()
{
	//delete gpu shaders
	ShaderManager.DeleteAllShaders();
	
	//close imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void BaseLayer::BaseRender()
{
	//draw layer
	Render();
	//draw imgui when not using camera
	if(!m_camera.GetUsingCamera())
		BaseimGUI();
}

void BaseLayer::BaseUpdate(const float& dt)
{
	//process window events
	m_renderer.GetWindow().UpdateWindow();
	//update camera
	m_camera.Update(dt);
	//update generic shader uniforms
	ShaderManager.UpdateAllShaders(m_camera.GetPerspectiveViewMat4(), m_camera.GetPosition());
	//update layer
	Update(dt);

}

void BaseLayer::BaseimGUI()
{
	//imgui frame setup
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//render imgui
	ImGUI();

	//imgui frame done
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void BaseLayer::InitGLAD()
{
	//pull opengl function pointers from driver
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize glad!\n";
	else
		std::cout << "GLAD loaded successfully!\n";
}

void BaseLayer::CreateShader()
{
	//create default shaders

	Shader shader;
	shader.Create("Data/Shaders/defaultVertexShader.vert", "Data/Shaders/defaultFragmentShader.frag");
	ShaderManager.AddShader("main", shader);

	Shader shader_2;
	shader_2.Create("Data/Shaders/Position-Color.vert", "Data/Shaders/Position-Color.frag");
	ShaderManager.AddShader("Position-Color", shader_2);

	Shader shader_3;
	shader_3.Create("Data/Shaders/DiscardAlpha.vert", "Data/Shaders/DiscardAlpha.frag");
	ShaderManager.AddShader("DiscardAlpha", shader_3);

	Shader shader_4;
	shader_4.Create("Data/Shaders/frameBufferQuad.vert", "Data/Shaders/frameBufferQuad.frag");
	ShaderManager.AddShader("frameBufferQuad", shader_4);

}

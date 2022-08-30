#include "BaseLayer.h"
#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()
#include"InputManager.h"
#define InputManager InputManager::GetInstance()

#include "external_libaries/include/imGUI/imgui.h"
#include "external_libaries/include/imGUI/imgui_impl_glfw.h"
#include "external_libaries/include/imGUI/imgui_impl_opengl3.h"

#include"Helper.h"
void BaseLayer::CreateLayer(const glm::vec<2, int> windowSize, const std::string& windowName)
{
	m_renderer.CreateWindowGLFW(windowSize.x, windowSize.y, windowName);
	InitGLAD();
	m_renderer.InitRenderer();
	InputManager.InitializeInputManager(m_renderer.GetWindow().GetWindowPtr());
	CreateShader();
	m_camera.Initialize(90.0f, static_cast<glm::vec2>(windowSize), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), 5.0f, .1f, glm::vec3(0.0f, 0.0f, 4.0f), m_renderer.GetWindow().GetWindowPtr());
	Helper::Status::DisplayUsefulInfo();
	InitializeImGui();
}

void BaseLayer::InitializeImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_renderer.GetWindow().GetWindowPtr(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void BaseLayer::DestroyLayer()
{
	ShaderManager.DeleteAllShaders();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void BaseLayer::BaseRender()
{
	Render();
	if(!m_camera.GetUsingCamera())
		BaseimGUI();
}

void BaseLayer::BaseUpdate(const float& dt)
{
	m_renderer.GetWindow().UpdateWindow();
	m_camera.Update(dt);
	ShaderManager.UpdateAllShaders(m_camera.GetPerspectiveViewMat4(), m_camera.GetPosition());
	Update(dt);

}

void BaseLayer::BaseimGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void BaseLayer::InitGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize glad!\n";
	else
		std::cout << "GLAD loaded successfully!\n";
}

void BaseLayer::CreateShader()
{
	Shader shader;
	shader.Create("Data/Shaders/defaultVertexShader.vert", "Data/Shaders/defaultFragmentShader.frag");
	ShaderManager.AddShader("main", shader);

	Shader shader_2;
	shader_2.Create("Data/Shaders/Position-Color.vert", "Data/Shaders/Position-Color.frag");
	ShaderManager.AddShader("Position-Color", shader_2);

}

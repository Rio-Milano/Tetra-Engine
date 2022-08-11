#include "BaseLayer.h"
#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()
#include"InputManager.h"
#define InputManager InputManager::GetInstance()

#include"Helper.h"
void BaseLayer::CreateLayer(const glm::vec<2, int> windowSize, const std::string& windowName)
{
	m_renderer.CreateWindow(windowSize.x, windowSize.y, windowName);
	InitGLAD();
	m_renderer.InitRenderer();
	InputManager.InitializeInputManager(m_renderer.GetWindow().GetWindowPtr());
	CreateShader();
	m_camera.Initialize(90.0f, static_cast<glm::vec2>(windowSize), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), 5.0f, .1f, glm::vec3(0.0f, 0.0f, 4.0f), m_renderer.GetWindow().GetWindowPtr());
	Helper::Status::DisplayUsefulInfo();
}

void BaseLayer::DestroyLayer()
{
	ShaderManager.DeleteAllShaders();
}

void BaseLayer::BaseUpdate(const float& dt)
{
	m_renderer.GetWindow().UpdateWindow();
	m_camera.Update(dt);
	ShaderManager.UpdateAllShaders(m_camera.GetPerspectiveViewMat4());
	Update(dt);

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
	shader.Create("Data/Shaders/vertexShader.vert", "Data/Shaders/fragmentShader.frag");
	ShaderManager.AddShader("main", shader);
}

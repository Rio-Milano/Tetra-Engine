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
	//
	CreateUniformBuffers();
	//create default shaders
	CreateShader();
	//setup camera with attributes
	m_camera.Initialize(90.0f, static_cast<glm::vec2>(windowSize), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), 5.0f, .1f, glm::vec3(0.0f, 0.0f, 0.0f), m_renderer.GetWindow().GetWindowPtr());
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
	
	ShaderManager.UpdateAllShaders(m_camera.GetPosition());

	UpdateUniformBuffers();
	
	Shader& skyBoxShader = ShaderManager.GetShader("SkyBox");
	glm::mat4 Projection_X_View_nt = m_camera.GetPerspectiveMat4() * glm::mat4(glm::mat3(m_camera.GetViewMat4()));
	skyBoxShader.SetUniformMat4f(skyBoxShader.GetLocation("Projection_X_View"), Projection_X_View_nt);

	//update layer
	Update(dt);

}

void BaseLayer::UpdateUniformBuffers()
{
	//Update the matrix UBO
	{
		const std::shared_ptr<UniformBufferObject>& Matricies = ShaderManager.GetUniformBufferObject("Matricies");
		glm::mat4 Projection_X_View = m_camera.GetPerspectiveMat4() * m_camera.GetViewMat4();
		Matricies->SetBufferElement("Projection_X_View", glm::value_ptr(Projection_X_View));
	}

	const std::shared_ptr<UniformBufferObject> World = ShaderManager.GetUniformBufferObject("World");
	float time = static_cast<float>(glfwGetTime());
	World->SetBufferElement("time", static_cast<void*>(&time));
}

void BaseLayer::CreateUniformBuffers()
{
	std::shared_ptr<UniformBufferObject> Matricies = std::make_shared<UniformBufferObject>(static_cast<GLsizei>(sizeof(glm::mat4)), 0, "Matricies");
	Matricies->SetElementData("Projection_X_View", UniformBufferObject::Element{sizeof(glm::mat4), 0});
	ShaderManager.AddUniformBufferObject("Matricies", Matricies);

	std::shared_ptr<UniformBufferObject> World = std::make_shared<UniformBufferObject>(static_cast<GLsizei>(sizeof(float)), 1, "World");
	World->SetElementData("time", UniformBufferObject::Element{sizeof(float), 0});
	ShaderManager.AddUniformBufferObject("World", World);

	size_t ubo_size = 80 * NUMBER_OF_LIGHTS;


	std::shared_ptr<UniformBufferObject> ubo = std::make_shared<UniformBufferObject>(static_cast<GLsizei>(ubo_size), 2, "Lights");

	GLsizei offset = 0;
	for (size_t i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		const size_t lightNumber = i + 1;

		//position
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Position", UniformBufferObject::Element{ 12, offset });
		
		//Color
		offset += 16;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Color", UniformBufferObject::Element{ 12, offset});
		
		//Direction
		offset += 16;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Direction", UniformBufferObject::Element{ 12, offset });

		//Intensity
		offset += 12;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Intensity", UniformBufferObject::Element{ 4, offset });
		
		//Range
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Range", UniformBufferObject::Element{ 4, offset });
		
		//Inner Cutoff
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":InnerCutoff", UniformBufferObject::Element{ 4, offset });
		
		//Inner Cutoff
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":OuterCutoff", UniformBufferObject::Element{ 4, offset });

		//Type
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Type", UniformBufferObject::Element{ 4, offset });
		
		//In Use
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":InUse", UniformBufferObject::Element{ 4, offset });

		offset += 16;
	}
	ShaderManager.AddUniformBufferObject("Lights", ubo);
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
	shader.Create("Data/Shaders/defaultVertexShader.vert", "Data/Shaders/defaultFragmentShader.frag", "Data/Shaders/defaultGeometryShader.geom");
	ShaderManager.AddShader("main", shader);

	Shader explode;
	explode.Create("Data/Shaders/defaultVertexShader.vert", "Data/Shaders/defaultFragmentShader.frag", "Data/Shaders/explodingGeometryShader.geom");
	ShaderManager.AddShader("explode", explode);

	Shader shader_2;
	shader_2.Create("Data/Shaders/Position-Color.vert", "Data/Shaders/Position-Color.frag");
	ShaderManager.AddShader("Position-Color", shader_2);

	Shader shader_3;
	shader_3.Create("Data/Shaders/DiscardAlpha.vert", "Data/Shaders/DiscardAlpha.frag");
	ShaderManager.AddShader("DiscardAlpha", shader_3);

	Shader shader_4;
	shader_4.Create("Data/Shaders/frameBufferQuad.vert", "Data/Shaders/frameBufferQuad.frag");
	ShaderManager.AddShader("frameBufferQuad", shader_4);

	Shader shader_5;
	shader_5.Create("Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag");
	ShaderManager.AddShader("SkyBox", shader_5);

	{
		Shader draw_normals_shader;
		draw_normals_shader.Create("Data/Shaders/draw_normals.vert", "Data/Shaders/draw_normals.frag", "Data/Shaders/draw_normals.geom");
		ShaderManager.AddShader("Draw-Normals", draw_normals_shader);
	}

}

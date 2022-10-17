#include "../H/BaseLayer.h"
#include"../../Managers/H/ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()
#include"../../Managers/H/InputManager.h"
#define InputManager InputManager::GetInstance()
#include "../../../external_libaries/include/imGUI/imgui.h"
#include "../../../external_libaries/include/imGUI/imgui_impl_glfw.h"
#include "../../../external_libaries/include/imGUI/imgui_impl_opengl3.h"
#include"../../Misc/H/Helper.h"

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
	//
	m_lightManager.Initialize();

	//setup camera with attributes
	m_camera.Initialize(90.0f, static_cast<glm::vec2>(windowSize), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), 5.0f, .1f, glm::vec3(0.0f, 10.0f, 0.0f), m_renderer.GetWindow().GetWindowPtr());
	//output gl state
	Helper::Status::DisplayUsefulInfo();
	//init gui lib
	InitializeImGui();

	m_grid = std::make_shared<BaseGrid>();
	m_grid->Init();

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
	PreRender(nullptr);
	Render(nullptr);
	//m_grid->Render(m_renderer, nullptr);
	PostRender();

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
	glm::vec3 camPos = m_camera.GetPosition();
	World->SetBufferElement("cameraPosition", &camPos);
}

void BaseLayer::CreateUniformBuffers()
{
	std::shared_ptr<UniformBufferObject> Matricies = std::make_shared<UniformBufferObject>(static_cast<GLsizei>(sizeof(glm::mat4)), 0, "Matricies");
	Matricies->SetElementData("Projection_X_View", UniformBufferObject::Element{sizeof(glm::mat4), 0});
	ShaderManager.AddUniformBufferObject("Matricies", Matricies);

	std::shared_ptr<UniformBufferObject> World = std::make_shared<UniformBufferObject>(static_cast<GLsizei>(16), 1, "World");
	World->SetElementData("cameraPosition", UniformBufferObject::Element{12, 0});
	World->SetElementData("time", UniformBufferObject::Element{ 4, 12});
	ShaderManager.AddUniformBufferObject("World", World);

	size_t ubo_size = 128 * NUMBER_OF_LIGHTS;


	std::shared_ptr<UniformBufferObject> ubo = std::make_shared<UniformBufferObject>(static_cast<GLsizei>(ubo_size), 2, "Lights");

	GLsizei offset = 0;
	for (size_t i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		const size_t lightNumber = i + 1;

		//position
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Position", UniformBufferObject::Element{ 12, offset });

		//Range
		offset += 12;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Range", UniformBufferObject::Element{ 4, offset });

		//Color
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Color", UniformBufferObject::Element{ 12, offset});
		
		//Inner Cutoff
		offset += 12;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":InnerCutoff", UniformBufferObject::Element{ 4, offset });

		//Direction
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Direction", UniformBufferObject::Element{ 12, offset });

		//Intensity
		offset += 12;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Intensity", UniformBufferObject::Element{ 4, offset });
		
		//LightSpace
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Light-Space", UniformBufferObject::Element{64, offset});

		//Outer Cutoff
		offset += 64;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":OuterCutoff", UniformBufferObject::Element{ 4, offset });

		//Type
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":Type", UniformBufferObject::Element{ 4, offset });
		
		//In Use
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":InUse", UniformBufferObject::Element{ 4, offset });

		//FarPlane
		offset += 4;
		ubo->SetElementData("Light:" + std::to_string(lightNumber) + ":farPlane", UniformBufferObject::Element{ 4, offset });

		offset += 4;
		
	}
	ShaderManager.AddUniformBufferObject("Lights", ubo);
}/*
 	float nearPlane;
	float farPlane;
 */

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

	//Main Shader
	{
		Shader shader;
		shader.Create("Data/Shaders/Main/defaultVertexShader.vert", "Data/Shaders/Main/defaultFragmentShader.frag", "Data/Shaders/Main/defaultGeometryShader.geom");
		ShaderManager.AddShader("main", shader);
	}

	//Explode Shader
	{
		Shader explode;
		explode.Create("Data/Shaders/Explosion/explode_primitives.vert", "Data/Shaders/Main/defaultFragmentShader.frag", "Data/Shaders/Explosion/explode_primitives.geom");
		ShaderManager.AddShader("explode", explode);
	}

	//Position Color
	{
		Shader shader_2;
		shader_2.Create("Data/Shaders/Basic Position Color/Position-Color.vert", "Data/Shaders/Basic Position Color/Position-Color.frag");
		ShaderManager.AddShader("Position-Color", shader_2);
	}

	//Low Alpha Discard Shader
	{
		Shader shader_3;
		shader_3.Create("Data/Shaders/Discard Alpha/DiscardAlpha.vert", "Data/Shaders/Discard Alpha/DiscardAlpha.frag");
		ShaderManager.AddShader("DiscardAlpha", shader_3);
	}

	//Post Processing Shader
	{
		Shader shader_4;
		shader_4.Create("Data/Shaders/Post Processing/frameBufferQuad.vert", "Data/Shaders/Post Processing/frameBufferQuad.frag");
		ShaderManager.AddShader("frameBufferQuad", shader_4);
	}

	//SkyBox Shader
	{
		Shader shader_5;
		shader_5.Create("Data/Shaders/SkyBox/SkyBox.vert", "Data/Shaders/SkyBox/SkyBox.frag");
		ShaderManager.AddShader("SkyBox", shader_5);
	}

	//Draw Normals Shader
	{
		Shader draw_normals_shader;
		draw_normals_shader.Create("Data/Shaders/Draw Normals/draw_normals.vert", "Data/Shaders/Draw Normals/draw_normals.frag", "Data/Shaders/Draw Normals/draw_normals.geom");
		ShaderManager.AddShader("Draw-Normals", draw_normals_shader);
	}

	//Shadow Mapping Shader
	{
		Shader shadowMapping;
		shadowMapping.Create("Data/Shaders/Shadow Mapping/shadowMapping.vert", "Data/Shaders/Shadow Mapping/shadowMapping.frag");
		ShaderManager.AddShader("Shadow-Mapping", shadowMapping);
	}

	//Point Shadows Shader
	{
		Shader pointShadow;
		pointShadow.Create("Data/Shaders/Point Shadows/pointShadows.vert", "Data/Shaders/Point Shadows/pointShadows.frag", "Data/Shaders/Point Shadows/pointShadows.geom");
		ShaderManager.AddShader("Point-Shadows", pointShadow);
	}
}

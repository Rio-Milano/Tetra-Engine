#include "SandBoxLayer.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()
#include"InputManager.h"
#define InputManager InputManager::GetInstance()
#include "external_libaries/include/imGUI/imgui.h"
#include"MeshManager.h"
#define MeshManager MeshManager::GetInstance()

void SandBoxLayer::Start()
{
	std::shared_ptr<Texture> t1 = std::make_shared<Texture>();
	t1->InitializeTexture("Data/Images/Box.png");
	TextureManager.AddTexture("Box", t1);

	std::shared_ptr<Texture> t2 = std::make_shared<Texture>();
	t2->InitializeTexture("Data/Images/BoxSpec.png");
	TextureManager.AddTexture("BoxSpec", t2);

	std::shared_ptr<Texture> t3 = std::make_shared<Texture>();
	t3->InitializeTexture("Data/Images/BoxEmission.png");
	TextureManager.AddTexture("BoxEmission", t3);


	backPack = MeshManager.LoadModel("Data/Models/backpack/backpack.obj");

	//modelRoot->m_transform_mat4 = glm::rotate(modelRoot->m_transform_mat4, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//modelRoot->m_transform_mat4 = glm::scale(modelRoot->m_transform_mat4, glm::vec3(0.005));

	//load in the seperate .obj files
	std::shared_ptr<Model>
		gun = MeshManager.LoadModel("Data/Models/AquaPig/gun.obj"),
		gun_base = MeshManager.LoadModel("Data/Models/AquaPig/gun_base.obj"),
		hull = MeshManager.LoadModel("Data/Models/AquaPig/hull.obj"),
		propeller = MeshManager.LoadModel("Data/Models/AquaPig/propeller.obj"),
		wing_left = MeshManager.LoadModel("Data/Models/AquaPig/wing_left.obj"),
		wing_right = MeshManager.LoadModel("Data/Models/AquaPig/wing_right.obj");

	if (!(gun && gun_base && hull && propeller && wing_left && wing_left)) 
		_ASSERT(false);

	//take the nodes from each model
	std::shared_ptr<ModelNode>
		ngun = gun->m_rootModelNode,
		ngun_base = gun_base->m_rootModelNode,
		nhull = hull->m_rootModelNode,
		npropeller = propeller->m_rootModelNode,
		nwing_left = wing_left->m_rootModelNode,
		nwing_right = wing_right->m_rootModelNode;

	std::shared_ptr<Texture> aquaPigTexture = std::make_shared<Texture>();
	aquaPigTexture->InitializeTexture("Data/Models/AquaPig/aqua_pig_1K.jpg");
	
	ngun->m_children[0]->m_meshes[0]->m_diffuse = aquaPigTexture;
	ngun_base->m_children[0]->m_meshes[0]->m_diffuse = aquaPigTexture;
	nhull->m_children[0]->m_meshes[0]->m_diffuse = aquaPigTexture;
	npropeller->m_children[0]->m_meshes[0]->m_diffuse = aquaPigTexture;
	nwing_left->m_children[0]->m_meshes[0]->m_diffuse = aquaPigTexture;
	nwing_right->m_children[0]->m_meshes[0]->m_diffuse = aquaPigTexture;


	//construct hirearchy
	nhull->AddChildren({ nwing_left, nwing_right, npropeller, ngun_base });
	ngun_base->AddChild(ngun);

	//construct transform hirearchy
	glm::mat4 Twing_right(1.0f);
	Twing_right = glm::translate(Twing_right, glm::vec3(-2.231, 0.272, -2.663));
	nwing_right->m_transform_mat4 = Twing_right;

	glm::mat4 Twing_left(1.0f);
	Twing_left = glm::translate(Twing_left, glm::vec3(2.231, 0.272, -2.663));
	nwing_left->m_transform_mat4 = Twing_left;

	glm::mat4 Tpropeller(1.0f);
	Tpropeller = glm::translate(Tpropeller, glm::vec3(0, 1.395, -3.616));
	Tpropeller = glm::rotate(Tpropeller, (float)glm::radians(90.0f), glm::vec3(1, 0, 0));
	npropeller->m_transform_mat4 = Tpropeller;

	glm::mat4 Tgun_base(1.0f);
	Tgun_base = glm::translate(Tgun_base, glm::vec3(0, 0.569, -1.866));
	ngun_base->m_transform_mat4 = Tgun_base;

	glm::mat4 Tgun(1.0f);
	Tgun = glm::translate(Tgun, glm::vec3(0, 1.506, 0.644));
	ngun->m_transform_mat4 = Tgun;



	aquaPig = hull;


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

	 backPack->m_rootModelNode->m_transform_mat4 = glm::rotate(backPack->m_rootModelNode->m_transform_mat4, (float)glm::radians(40.0*dt), glm::vec3(0.0f, 1.0f, 0.0f));

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

	m_lightManager.UpdateShader(dt);
	
	static float theta = 0.0f;

	std::shared_ptr<ModelNode> propellerNode = aquaPig->FindNode("propeller.obj");
	glm::mat4 Tpropeller(1.0f);
	Tpropeller = glm::rotate(Tpropeller, (float)glm::radians(theta), glm::vec3(0, 1, 0));
	propellerNode->m_nodeTransform = Tpropeller;

	std::shared_ptr<ModelNode> hullNode = aquaPig->m_rootModelNode;
	glm::mat4 hull(1.0f);
	hull = glm::translate(hull, glm::vec3(8, 0, 8));
	hull = glm::rotate(hull, (float)glm::radians(sin(glfwGetTime()))*10.f, glm::vec3(1, 0, 1));
	hullNode->m_nodeTransform = hull;

	std::shared_ptr<ModelNode> gunNode = aquaPig->FindNode("gun");
	glm::mat4 Tgun(1.0f);
	Tgun = glm::rotate(Tgun, (float)glm::radians(sin(glfwGetTime())) * 60.f, glm::vec3(1, 0, 0));
	gunNode->m_nodeTransform = Tgun;

	theta += 400.0f * dt;

}

void SandBoxLayer::Render()
{

	m_lightManager.DrawLights(m_renderer);
	m_entity.Render(m_renderer);
	backPack->Render(m_renderer);

	aquaPig->Render(m_renderer);
}

void SandBoxLayer::End()
{
}

void SandBoxLayer::ImGUI()
{
	static bool LightingGUIOpen = true;
	static bool SystemGUIOpen = true;

	float buttonX = 140.f;
	float buttonY = 20.f;

	//Lighting GUI
	{

		if (LightingGUIOpen)
		{
			ImGui::Begin("Lights");

			if (ImGui::Button("Close", ImVec2(buttonX, buttonY)))
				LightingGUIOpen = false;

			for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
			{
				if (ImGui::BeginCombo(std::string("Light :" + std::to_string(i + 1)).c_str(), "", ImGuiComboFlags_HeightLargest))
				{
					Light& light = m_lightManager.GetLight(i);

					ImGui::Checkbox("Light in use", &light.m_inUse);

					if (light.m_lightType == LightType::Directional)
					{
						ImGui::Text("Directional Light");

						ImGui::SliderFloat3("Direction", &light.m_direction.x, MIN_DIRECTION_XYZ, MAX_DIRECTION_XYZ);
						ImGui::ColorPicker3("Color", &light.m_lightColor[0]);
						ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
					}
					else if (light.m_lightType == LightType::Point)
					{
						ImGui::Text("Point Light");

						ImGui::SliderFloat3("Position", &light.m_position.x, MIN_XYZ, MAX_XYZ);
						ImGui::ColorPicker3("Color", &light.m_lightColor[0]);
						ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
						ImGui::SliderFloat("Light range", &light.m_range, 0, MAX_RANGE);

					}
					else if (light.m_lightType == LightType::Spot)
					{
						ImGui::Text("Spot Light");

						ImGui::SliderFloat3("Position", &light.m_position.x, MIN_XYZ, MAX_XYZ);
						ImGui::SliderFloat3("Direction", &light.m_direction.x, MIN_DIRECTION_XYZ, MAX_DIRECTION_XYZ);
						ImGui::ColorPicker3("Color", &light.m_lightColor[0]);
						ImGui::SliderFloat("Light intensity", &light.m_lightIntensity, MIN_COLOR, MAX_COLOR);
						ImGui::SliderFloat("Inner Cutoff Angle", &light.m_innerCutOffAngle, MIN_CONE_ANGLE, MAX_CONE_ANGLE);
						ImGui::SliderFloat("Outer Cutoff Angle", &light.m_outerCutOffAngle, MIN_CONE_ANGLE, MAX_CONE_ANGLE);
						ImGui::SliderFloat("Light range", &light.m_range, 0, MAX_RANGE);
						ImGui::Checkbox("Simulate Light", &light.simulateLight);
						if (light.simulateLight)
						{
							ImGui::SliderFloat("Radius", &light.radius, 0.0f, 20.f);
							ImGui::SliderFloat("Rotation Speed", &light.rotationSpeed, 0.0f, 100.0f);
						}
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
			ImGui::End();
		}
	}


	///Control ImGui
	{
		ImGui::Begin("Controll");
		ImGui::Checkbox("Rendering", &SystemGUIOpen);
		ImGui::Checkbox("Lighting", &LightingGUIOpen);
		if (ImGui::Button("Exit"))
		{
			glfwSetWindowShouldClose(m_renderer.GetWindow().GetWindowPtr(), true);
		}
		ImGui::End();

	}
}
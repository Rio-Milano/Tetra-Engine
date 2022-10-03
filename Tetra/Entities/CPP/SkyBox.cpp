#include "../H/SkyBox.h"
#include"../../Core/Structure/H/Mesh.h"
#include"../../OpenGL_Abstraction/H/CubeMap.h"
#include"../../Core/Graphics/H/Renderer.h"
#include"../../Core/Managers/H/TextureManager.h"
#define TextureManager TextureManager::GetInstance()

SkyBox::SkyBox(const std::string& entityName)
	:Entity(entityName)
{
}

SkyBox::~SkyBox()
{
}


void SkyBox::SetFaceLocations(const std::vector<std::string>& faceLocations)
{
	m_faceLocations = faceLocations;
}

void SkyBox::Init()
{
	//thease position vectors will also be used as direction vectors for sampling the cube map
	std::vector<glm::vec3>* positions = new std::vector<glm::vec3>
	{
		//back face
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		// front face
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left
		// left face
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-right
		// right face
		glm::vec3(0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		// bottom face
		glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // top-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
		// top face
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(0.5f, 0.5f, 0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, 0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(-0.5f, 0.5f, 0.5f), // bottom-left
	};

	m_skyBoxMesh = std::make_shared<Mesh>();
	m_skyBoxMesh->GenerateMesh(positions, {}, {}, {}, {}, 1, GL_STATIC_DRAW);
	m_skyBoxMesh->SetFaceCullingFlag(false);

	m_cubeMapForSkyBox = std::make_shared<CubeMap>();
	m_cubeMapForSkyBox->InitializeCubeMap(m_faceLocations);

	TextureManager.SetActiveSkyBoxCubeMap(m_cubeMapForSkyBox);
}

void SkyBox::Update(const float& dt)
{
}

void SkyBox::Render(Renderer& renderer, Shader* overideShader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapForSkyBox->GetCubeMapTextureID());

	Shader* skyBoxShader = overideShader ? overideShader : &ShaderManager.GetShader("SkyBox");
	skyBoxShader->SetUniform1i(skyBoxShader->GetLocation("cubeMap"), 0);
	
	glDepthFunc(GL_LEQUAL);
	renderer.RenderMesh(*m_skyBoxMesh.get(), glm::mat4(1.0f), *skyBoxShader);
	glDepthFunc(GL_LESS);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

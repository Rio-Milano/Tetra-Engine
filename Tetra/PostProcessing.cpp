#include "PostProcessing.h"
#include"Mesh.h"
#include"Renderer.h"
#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()

#include<vector>

PostProcessing::PostProcessing(const glm::vec2& viewPortSize)
	: m_frameBuffer(viewPortSize), m_quad(nullptr)
{

	m_quad = std::make_shared<Mesh>();
	std::vector<glm::vec3>* positions = new std::vector<glm::vec3>
	{
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	};
	std::vector<glm::vec2>* texCoords = new std::vector<glm::vec2>
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	};

	//generate the mesh for the quad
	m_quad->GenerateMesh(positions, {}, texCoords, {}, {}, 1, GL_STATIC_DRAW, "frameBufferQuad");
	//give it a name
	m_quad->SetMeshName("Quad");

	//the quad needs the diffuse texture to be color buffer of frame buffer yet it must be in a texture object so we create one
	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
	Texture::TextureAttributes& attributes = newTexture->GetTextureAttributes();
	attributes.textureID = m_frameBuffer.GetColorBufferID();
	attributes.validTexture = true;

	m_quad->GetMaterial()->m_diffuse = newTexture;
}

void PostProcessing::Render_To_Off_Screen_Buffer()
{
	//set the frame buffer as current frame buffer
	m_frameBuffer.SetFrameBuffer();

	//any render calls made directly after this call will be rendered to the framebuffer
}



void PostProcessing::Render_FrameBuffer(Renderer& renderer)
{
	//switch back to default frame buffer
	m_frameBuffer.ResetFrameBuffer();

	Shader& frameBufferQuadShader = ShaderManager.GetShader("frameBufferQuad");
	frameBufferQuadShader.SetUniform1b(frameBufferQuadShader.GetLocation("invertFragColor"), m_config.m_enableColorBufferInversion);
	frameBufferQuadShader.SetUniform1b(frameBufferQuadShader.GetLocation("greyScaleFragColor"), m_config.m_enableGreyScaleColorBuffer);
	frameBufferQuadShader.SetUniform1f(frameBufferQuadShader.GetLocation("XsamplingOffset"), m_config.m_XsampleOffsetMagnitude);
	frameBufferQuadShader.SetUniform1f(frameBufferQuadShader.GetLocation("YsamplingOffset"), m_config.m_YsampleOffsetMagnitude);
	frameBufferQuadShader.SetUniform1f(frameBufferQuadShader.GetLocation("sharpenKernel"), m_config.m_enableSharpeningKernel);
	frameBufferQuadShader.SetUniform1f(frameBufferQuadShader.GetLocation("weirdKernel"), m_config.m_enableWeirdKernel);
	frameBufferQuadShader.SetUniform1f(frameBufferQuadShader.GetLocation("blurKernel"), m_config.m_enableBlurKernel);
	frameBufferQuadShader.SetUniform1f(frameBufferQuadShader.GetLocation("edgeDetectionKernel"), m_config.m_edgeDetectionKernel);

	renderer.RenderMesh(*m_quad.get(), glm::mat4(1.0f));
}



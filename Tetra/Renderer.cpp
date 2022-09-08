#include "Renderer.h"
#include"Entity.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()
#include"Texture.h"
#include"CubeMap.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

void Renderer::CreateWindowGLFW(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	//create window
	m_window.CreateWindowGLFW(renderWindowWidth, renderWindowHeight, renderWindowName);
}

void Renderer::UpdateViewportWhenWindowSizeChanged(GLFWwindow* window, int windowWidth, int windowHeight)
{
	//when window size is changed set the view port size to the new window size
	glViewport(0, 0, windowWidth, windowHeight);
}

void Renderer::InitRenderer()
{
	//init the view port size 
	int winWidth, winHeight;
	glfwGetWindowSize(m_window.GetWindowPtr(), &winWidth, &winHeight);
	glViewport(0, 0, winWidth, winHeight);

	//set call back for viewport resize
	glfwSetFramebufferSizeCallback(m_window.GetWindowPtr(), UpdateViewportWhenWindowSizeChanged);

	glEnable(GL_DEPTH_TEST);//enable depth test
	glDepthMask(GL_TRUE);//the depth buffer is writable
	glDepthFunc(GL_LESS);//make fragments with lower depth be drawn in front of fragments with greater depth


	glEnable(GL_BLEND);//enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//set the way we blend the color buffer and fragment shader output
	glBlendEquation(GL_FUNC_ADD);//destColor * (1 - srcA) ADD sourceColor * srcA

}






void Renderer::RenderMesh(const Mesh& mesh, const glm::mat4& worldMat)
{
	if (mesh.m_material->m_blendingEnabled)
	{
		m_transparentMeshes.emplace_back(std::make_pair<const Mesh*, glm::mat4>(&mesh, glm::mat4(worldMat)));
		return;
	}
	//get the shader for the mesh
	Shader& shader = ShaderManager.GetShader(mesh.m_programName);
	
	//set the word matrix for the mesh
	shader.SetUniformMat4f(shader.GetLocation("worldMat"), worldMat);
	
	//calculate the normal matrix that allows for unballanced scaling so normals arent distorted
	glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(worldMat)));
	//set normal matrix in shader
	shader.SetUniformMat3f(shader.GetLocation("normalMat"), normalMat);

	//get the material of the mesh
	std::shared_ptr<Material> meshMaterial = mesh.GetMaterial();

	//set the material properties in the shader
	shader.SetUniform3fv(shader.GetLocation("material.defaultDiffuseColor"), meshMaterial->m_defaultDiffuseColor);
	shader.SetUniform3fv(shader.GetLocation("material.defaultSpecularColor"), meshMaterial->m_defaultSpecularColor);
	shader.SetUniform1f(shader.GetLocation("material.ambientIntensity"), meshMaterial->m_ambientIntensity);
	shader.SetUniform1f(shader.GetLocation("material.specularIntensity"), meshMaterial->m_specularIntensity);
	shader.SetUniform1f(shader.GetLocation("material.emissionRange"), meshMaterial->emissionRange);
	shader.SetUniform1b(shader.GetLocation("material.discardLowAlphaFragment"), meshMaterial->m_discardLowAlphaFragments);
	shader.SetUniform1i(shader.GetLocation("material.reflectionType"), meshMaterial->m_reflectionType);
	shader.SetUniform1i(shader.GetLocation("material.reflectionType"), meshMaterial->m_reflectionType);

	//get the tetxure pointer for the mesh diffuse map
	const std::shared_ptr<Texture>& diffuseTexture = meshMaterial->m_diffuse;
	//if the texture is valid and if loaded successfully
	if (diffuseTexture != nullptr && diffuseTexture->GetTextureAttributes().validTexture)
	{
		//set active texture unit to 0
		glActiveTexture(GL_TEXTURE0);
		//bind diffuse texture to unit 0
		glBindTexture(GL_TEXTURE_2D, diffuseTexture->GetTextureAttributes().textureID);
		//set the diffuse texture unit to 0 in shader
		shader.SetUniform1i(shader.GetLocation("material.diffuseMap"), 0);
		//set diffuse flag to true
		shader.SetUniform1b(shader.GetLocation("material.hasDiffuseMap"), true);
	}
	else
	{
		//if mesh does not have a diffuse texture then set diffuse flag to false
		shader.SetUniform1b(shader.GetLocation("material.hasDiffuseMap"), false);
	}
	//get the tetxure pointer for the mesh spec map
	const std::shared_ptr<Texture>& specularTexture = meshMaterial->m_specular;
	//if texture is valid and loaded successfully
	if (specularTexture != nullptr && specularTexture->GetTextureAttributes().validTexture)
	{
		//set active texture unit to 1
		glActiveTexture(GL_TEXTURE1);
		//bind spec texture to unit 1
		glBindTexture(GL_TEXTURE_2D, specularTexture->GetTextureAttributes().textureID);
		//set texture unit of spec map to 1
		shader.SetUniform1i(shader.GetLocation("material.specularMap"), 1);
		//set spec material flag to true
		shader.SetUniform1b(shader.GetLocation("material.hasSpecularMap"), true);
	}
	else
	{
		//if mesh has no spec map then set spec material flag to false
		shader.SetUniform1b(shader.GetLocation("material.hasSpecularMap"), false);
	}

	//get emission texture from mesh
	const std::shared_ptr<Texture>& emissionTexture = meshMaterial->m_emission;
	//if emission texture is valid and loaded successfully
	if (emissionTexture != nullptr && emissionTexture->GetTextureAttributes().validTexture)
	{
		//set active texture unit to 2
		glActiveTexture(GL_TEXTURE2);
		//bind emission texture to unit 2
		glBindTexture(GL_TEXTURE_2D, emissionTexture->GetTextureAttributes().textureID);
		//set emission unit to 2
		shader.SetUniform1i(shader.GetLocation("material.emissionMap"), 2);
		//set emission material flag to true
		shader.SetUniform1b(shader.GetLocation("material.hasEmissionMap"), true);
	}
	else
	{
		//if mesh has no emission material then set material flag to false
		shader.SetUniform1b(shader.GetLocation("material.hasEmissionMap"), false);
	}

	if (mesh.GetMaterial()->m_mapToEnviroment && TextureManager.GetSkyBoxCubeMap())
	{
		shader.SetUniform1b(shader.GetLocation("material.mapToEnviroment"), true);
		shader.SetUniform1b(shader.GetLocation("material.hasCubeMap"), true);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager.GetSkyBoxCubeMap()->GetCubeMapTextureID());
		shader.SetUniform1i(shader.GetLocation("material.cubeMap"), 3);
		shader.SetUniform1i(shader.GetLocation("material.reflectionType"), meshMaterial->m_reflectionType);
		shader.SetUniform1f(shader.GetLocation("fromRefractiveIndex"), RefractiveIndex::Air);
		shader.SetUniform1f(shader.GetLocation("toRefractiveIndex"), meshMaterial->m_toRefractiveIndex);
	}
	else
	{
		shader.SetUniform1b(shader.GetLocation("material.mapToEnviroment"), false);
		shader.SetUniform1b(shader.GetLocation("material.hasCubeMap"), false);

	}

	//bind to the vertex array of the mesh
	glBindVertexArray(mesh.m_VAO);

	//define a vertex count
	GLsizei vertexCount = 0ull;
	if (mesh.customVertex)//if vertices were specified differently to the default way
		vertexCount = static_cast<GLsizei>(mesh.m_vertexCount);//use the vertex count of the mesh
	else
		//otherwise pull the vertex count from the vert vector
		vertexCount = static_cast<GLsizei>(mesh.m_verticies.size());
	
	//apply culling 
	if (mesh.m_useCullingCCWBack)
	{
		glEnable(GL_CULL_FACE);//turn on face culling
		glFrontFace(GL_CCW);//set the front face winding order to counter clock wise as it is by default
		glCullFace(GL_BACK);//cull any primitives defined in the clock wise order
	}
	else
	{
		glDisable(GL_CULL_FACE);//turn of culling, idealy used for openbox meshes
	}

	//decode the draw type of the mesh
	switch (mesh.m_drawType)
	{
	case 0://if drawing triangles and using indexed buffer then render using elements
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.m_elements.size()), GL_UNSIGNED_INT, 0);
		break;
	case 1://if drawing triangles and using raw verticies
		glDrawArrays(GL_TRIANGLES,  0, static_cast<GLsizei>(vertexCount));
		break;
	case 2://if drawing lines and using raw verticies
		glDrawArrays(GL_LINES, 0, vertexCount);
	default:
		break;
	}
	//set all texture units to 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	//set default texture unit
	glActiveTexture(GL_TEXTURE0);

	//unbind from vao
	glBindVertexArray(0);
}

void Renderer::RenderTransparentMeshes(const glm::vec3& cameraPosition)
{
	std::sort(m_transparentMeshes.begin(), m_transparentMeshes.end(),
		[&](const std::pair<const Mesh*, glm::mat4>& lhs, const std::pair<const Mesh*, glm::mat4>& rhs)
		{
			glm::vec3 lhsPos = glm::vec3(lhs.second[3]);
			float lhsDistance = glm::length(lhsPos - cameraPosition);

			glm::vec3 rhsPos = glm::vec3(rhs.second[3]);
			float rhsDistance = glm::length(rhsPos - cameraPosition);

			return (lhsDistance > rhsDistance) ? true : false;
		});

	for (const std::pair<const Mesh*, glm::mat4>& object : m_transparentMeshes)
	{
		object.first->m_material->m_blendingEnabled = false;
		this->RenderMesh(*object.first, object.second);
		object.first->m_material->m_blendingEnabled = true;

	}

	m_transparentMeshes.clear();
}







void Renderer::StartRendering()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::EndRendering()
{

	glfwSwapBuffers(m_window.GetWindowPtr());
}


Window& Renderer::GetWindow()
{
	return m_window;
}

void Renderer::SetClearColor(const glm::vec4& clearColor)
{
	m_clearColor = clearColor;
}


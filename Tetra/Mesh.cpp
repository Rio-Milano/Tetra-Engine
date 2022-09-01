#include "Mesh.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include<iostream>




void Mesh::GenerateMesh
	(
		const std::vector<glm::vec3>& positions, 
		const std::vector<glm::vec3>& normals, 
		const std::vector<glm::vec2>& textureCords,
		const std::vector<GLuint>& elements, 

		const std::string& textureName, 
		const std::string& specularName, 
		const std::string& emissionName, 

		const GLuint& drawType, 
		const GLenum& usage,
		const std::string& programName
	)
{
	StartMesh(drawType, programName, elements);

	ConstructVerticiesFromRawData(positions, textureCords, normals);

	EndMesh(textureName, specularName, emissionName);
}


void Mesh::EndMesh(const std::string& textureName, const std::string& specularName, const std::string& emissionName, const GLenum& usage)
{
	SendVertexDataToGPU(usage);

	ProcessTextures(textureName, specularName, emissionName);
}

void Mesh::ProcessTextures(const std::string& textureName, const std::string& specularName, const std::string& emissionName)
{

	//bind texture maps to mesh
	if (textureName.size())
		m_diffuse = TextureManager.GetTexture(textureName);

	if (specularName.size())
		m_specular = TextureManager.GetTexture(specularName);

	if (emissionName.size())
		m_emission = TextureManager.GetTexture(emissionName);
}










void Mesh::GenerateMesh
(
	const std::vector<Vertex>& verticies,
	const std::vector<GLuint>& elements,

	const std::shared_ptr<Texture>& diffuseTexture,
	const std::shared_ptr<Texture>& specularTexture,
	const std::shared_ptr<Texture>& emissiveTexture,


	const GLuint& drawType,
	const GLenum& usage,
	const std::string& programName
)
{
	StartMesh(drawType, programName, elements);

	m_verticies = verticies;

	EndMesh(diffuseTexture, specularTexture, emissiveTexture);}


void Mesh::EndMesh
(
	const std::shared_ptr<Texture>& diffuseTexture,
	const std::shared_ptr<Texture>& specularTexture,
	const std::shared_ptr<Texture>& emissiveTexture,
	const GLenum& usage
)
{
	SendVertexDataToGPU(usage);
	ProcessTextures(diffuseTexture, specularTexture, emissiveTexture);
}

void Mesh::ProcessTextures(const std::shared_ptr<Texture>& diffuseTexture, const std::shared_ptr<Texture>& specularTexture, const std::shared_ptr<Texture>& emissiveTexture)
{
	m_diffuse = diffuseTexture;
	m_specular = specularTexture;
	m_emission = emissiveTexture;
}





void Mesh::StartMesh(const GLuint& drawType,const std::string& programName, const std::vector<GLuint>& elements)
{
	//assign general mesh data
	m_VAO = StartVAO();
	m_drawType = drawType;
	m_programName = programName;

	//assign vertex data
	m_elements = elements;
}

void Mesh::ConstructVerticiesFromRawData(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& textureCords, const std::vector<glm::vec3>& normals)
{
	m_verticies.resize(positions.size());
	for (size_t i = 0; i < positions.size(); i++)
	{
		Vertex& vertex = m_verticies[i];

		if (positions.size())
			vertex.position = positions[i];

		if (textureCords.size())
			vertex.textureCord = textureCords[i];

		if (normals.size())
			vertex.normal = normals[i];
	}
}

void Mesh::SendVertexDataToGPU(const GLenum& usage)
{
	GLsizei vertexSize = sizeof(Vertex);

	//Send vertex data to gpu
	CreateBuffer(GL_ARRAY_BUFFER, m_verticies.size() * vertexSize, m_verticies.data(), usage);

	//setup position stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);

	//setup texture cord stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_TEXTURE_CORD, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex, textureCord));

	//setup normal stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex, normal));

	EndBuffer(GL_ARRAY_BUFFER);

	//set up elements
	CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_elements.size(), m_elements.data(), GL_STATIC_DRAW);

}











GLuint Mesh::StartVAO()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	return VAO;
}

void Mesh::EndVAO()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage)
{
	//use this function inside a VAO bind

	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(target, VBO);
	glBufferData(target, size, data, usage);

}
void Mesh::EndBuffer(const GLenum& target)
{
	glBindBuffer(target, 0);
}
;

void Mesh::CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, componentSize, componentType, normalized, stride, offset);
}
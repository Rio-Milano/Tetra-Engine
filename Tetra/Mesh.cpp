#include "Mesh.h"

#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()



void Mesh::GenerateMesh(Mesh& mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& textureCords, const std::string& textureName, const GLuint& drawType, const std::vector<GLuint>& elements)
{
	m_VAO = StartVAO();
	m_numberOfElements = elements.size();
	m_drawType = drawType;
	m_vertexCount = static_cast<GLuint>(positions.size());

	CreateBuffer(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	m_textureID = TextureManager.GetTexture(textureName).GetTextureAttributes().textureID;

	CreateBuffer(GL_ARRAY_BUFFER, textureCords.size() * sizeof(glm::vec2), textureCords.data(), GL_STATIC_DRAW);
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, 1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	CreateBuffer(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, 2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), (void*)elements.data(), GL_STATIC_DRAW);

	EndVAO();
}

void Mesh::SetProgramID(const GLuint& programID)
{
	m_programID = programID;
}

void Mesh::SetTextureID(const GLuint& textureID)
{
	m_textureID = textureID;
}

const GLuint& Mesh::GetVAO() const
{
	return m_VAO;
}

const GLenum& Mesh::GetDrawType() const
{
	return m_drawType;
}

const GLsizei& Mesh::GetNumberOfElements() const
{
	return static_cast<GLsizei>(m_numberOfElements);
}

const GLsizei& Mesh::GetVertexCount() const
{
	return static_cast<GLsizei>(m_vertexCount);
}

const GLuint& Mesh::GetProgramID() const
{
	return m_programID;
}

const GLuint& Mesh::GetTextureID() const
{
	return m_textureID;
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

};

void Mesh::CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, componentSize, componentType, normalized, stride, offset);

	glBindBuffer(target, 0);
}
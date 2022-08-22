#ifndef MESH_H
#define MESH_H

#include"glmIncludes.h"

#include<glad/glad.h>
#include<vector>
#include<string>

class Mesh
{
	friend class Renderer;

public:
	Mesh() = default;
	~Mesh() = default;

	void GenerateMesh(std::vector<glm::vec3>& positions, const std::string& textureName, const std::string& specularName, const std::string& emissionName, const GLuint& drawType, const std::vector<GLuint>& elements = std::vector<GLuint>{}, const std::vector<glm::vec3>& normals = std::vector<glm::vec3>{}, const std::vector<glm::vec2>& textureCords = std::vector<glm::vec2>{});

	const std::string& GetProgramName()const;
	const GLuint& GetTextureID()const;
	const GLuint& GetVAO()const;
	const GLenum& GetDrawType()const;
	const size_t& GetNumberOfElements()const;
	const size_t& GetVertexCount()const;
	const bool& GetHasTexture()const;
	const bool& GetHasSpecular()const;

	void SetProgramName(const std::string& programName);
	void SetTextureID(const GLuint& textureID);

protected:
	float m_ambientIntensity{0.1f};
	float m_specularIntensity{3.f};
	glm::vec3 m_defaultDiffuseColor{ 0.1f, 0.1f, 0.1f };
	glm::vec3 m_defaultSpecularColor{ 0.0f, 1.0f, 0.0f };
	float emissionRange{40.0f};

private:
	GLuint StartVAO();
	void EndVAO();
	void CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage);
	void CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset);

	GLuint
		m_VAO,
		m_textureID,
		m_specularID,
		m_emissionID,
		m_drawType;
	
	size_t
		m_numberOfElements,
		m_vertexCount;

	std::string
		m_programName;

	bool m_hasBoundTexture{ false };
	bool m_hasBoundSpecular{ false };
	bool m_hasBoundEmission{ false };
};

#endif
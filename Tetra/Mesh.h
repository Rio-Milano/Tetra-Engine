#ifndef MESH_H
#define MESH_H

#include"glmIncludes.h"

#include<glad/glad.h>
#include<vector>
#include<string>

class Mesh
{
public:
	Mesh() = default;	
	~Mesh() = default;

	void GenerateMesh(Mesh& mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& textureCords, const std::string& textureName, const GLuint& drawType, const std::vector<GLuint>& elements = std::vector<GLuint>{});

	const GLuint& GetProgramID()const;
	const GLuint& GetTextureID()const;
	const GLuint& GetVAO()const;
	const GLenum& GetDrawType()const;
	const GLsizei& GetNumberOfElements()const;
	const GLsizei& GetVertexCount()const;

	void SetProgramID(const GLuint& programID);
	void SetTextureID(const GLuint& textureID);


private:
	GLuint StartVAO();
	void EndVAO();
	void CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage);
	void CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset);

	GLuint
		m_VAO,
		m_textureID,
		m_programID,
		m_drawType;
	
	size_t
		m_numberOfElements,
		m_vertexCount;;
};

#endif
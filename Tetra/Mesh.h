#ifndef MESH_H
#define MESH_H

#include"glmIncludes.h"

#include<glad/glad.h>
#include<vector>
#include<string>
#include<memory>
#include"Texture.h"


#define SHADER_LAYOUT_INDEX_POSITION		0
#define SHADER_LAYOUT_INDEX_TEXTURE_CORD	1
#define SHADER_LAYOUT_INDEX_NORMAL			2

struct Vertex
{
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec2 textureCord{0.0f, 0.0f};
	glm::vec3 normal{0.0f, 0.0f, 0.0f};

};
class Mesh
{
	friend class Renderer;

public:

	Mesh() = default;
	~Mesh() = default;

	void GenerateMesh
	(
		const std::vector<glm::vec3>& positions = {},
		const std::vector<glm::vec3>& normals = {},
		const std::vector<glm::vec2>& textureCords = {},
		const std::vector<GLuint>& elements = {},

		const std::string& textureName = "",
		const std::string& specularName = "",
		const std::string& emissionName = "",

		const GLuint& drawType = 1,
		const GLenum& usage = GL_STATIC_DRAW,
		const std::string& programName = "main"
	);

	
	void GenerateMesh
	(
		const std::vector<Vertex>& verticies = {},
		const std::vector<GLuint>& elements = {},

		const std::shared_ptr<Texture>& diffuseTexture = nullptr,
		const std::shared_ptr<Texture>& specularTexture = nullptr,
		const std::shared_ptr<Texture>& emissiveTexture = nullptr,


		const GLuint& drawType = 1,
		const GLenum& usage = GL_STATIC_DRAW,
		const std::string& programName = "main"
	);

protected:
	//default values
	float m_ambientIntensity{0.1f};//amount of ambient light we can reflect
	float m_specularIntensity{3.f};//amount of specular lighting we can reflect
	glm::vec3 m_defaultDiffuseColor{ 0.1f, 0.1f, 0.1f };//default colors when no texture maps are avaliable
	glm::vec3 m_defaultSpecularColor{ 0.0f, 1.0f, 0.0f };
	float emissionRange{40.0f};//range on emission light source (treated as a light but only attenuation is applied)

private:
	void StartMesh(const GLuint& drawType, const std::string& programName, const std::vector<GLuint>& elements);
	
	void ConstructVerticiesFromRawData(const std::vector<glm::vec3>& positions = {}, const std::vector<glm::vec2>& textureCords = {}, const std::vector<glm::vec3>& normals = {});
	void SendVertexDataToGPU(const GLenum& usage = GL_STATIC_DRAW);
	
	void ProcessTextures(const std::shared_ptr<Texture>& diffuseTexture = nullptr, const std::shared_ptr<Texture>& specularTexture = nullptr, const std::shared_ptr<Texture>& emissiveTexture = nullptr);
	void ProcessTextures(const std::string& textureName = "", const std::string& specularName = "", const std::string& emissionName = "");

	void EndMesh(const std::shared_ptr<Texture>& diffuseTexture = nullptr,	const std::shared_ptr<Texture>& specularTexture = nullptr,	const std::shared_ptr<Texture>& emissiveTexture = nullptr, const GLenum& usage = GL_STATIC_DRAW);
	void EndMesh(const std::string& textureName = "", const std::string& specularName = "", const std::string& emissionName = "", const GLenum& usage = GL_STATIC_DRAW);

	GLuint StartVAO();
	void EndVAO();
	void CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage);
	void EndBuffer(const GLenum& target);
	void CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset);

	//mesh vertices and indexing
	std::vector<Vertex> m_verticies;
	std::vector<GLuint> m_elements;


	//general mesh data
	GLuint
		m_VAO,
		m_drawType;

	//shader link
	std::string
		m_programName;

	//textures
	std::shared_ptr<Texture>
		m_diffuse,
		m_specular,
		m_emission;
};

#endif
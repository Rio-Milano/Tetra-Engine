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

//simple vertex representation
struct Vertex
{
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec2 textureCord{0.0f, 0.0f};
	glm::vec3 normal{0.0f, 0.0f, 0.0f};
};


//all materials encapsulated 
struct Material
{
	std::shared_ptr<Texture>
		m_diffuse,
		m_specular,
		m_emission;


	//default values
	float m_ambientIntensity{ 0.1f };//amount of ambient light we can reflect
	float m_specularIntensity{ 3.f };//amount of specular lighting we can reflect

	glm::vec3 m_defaultDiffuseColor{ 1.0f, 1.0f, 1.0f };//default colors when no texture maps are avaliable
	glm::vec3 m_defaultSpecularColor{ 1.0f, 1.0f, 1.0f };
	
	float emissionRange{ 40.0f };//range on emission light source (treated as a light but only attenuation is applied)
};



//class for representing a collection of attributes opengl can use to display something
class Mesh
{
	friend class Renderer;//!!!!!!!!
	friend class Line;

public:
	//ctors/dtors
	Mesh();
	~Mesh() = default;

	//for generating a user defined mesh
	void GenerateMesh
	(
		const std::vector<glm::vec3>& positions = {},
		const std::vector<glm::vec3>& normals = {},
		const std::vector<glm::vec2>& textureCords = {},
		const std::vector<GLuint>& elements = {},

		const GLuint& drawType = 1,
		const GLenum& usage = GL_STATIC_DRAW,
		const std::string& programName = "main"
	);

	//for generating an assimp defined mesh
	void GenerateMesh
	(
		const std::vector<Vertex>& verticies = {},
		const std::vector<GLuint>& elements = {},

		const GLuint& drawType = 1,
		const GLenum& usage = GL_STATIC_DRAW,
		const std::string& programName = "main"
	);

	//getters
	const std::string& GetMeshName() const { return m_meshName; };
	const std::shared_ptr<Material>& GetMaterial()const;
	const std::string& GetProgramName()const;
	//setters
	void SetMeshName(const std::string& meshName) { m_meshName = meshName; };
	void SetMaterial(const std::shared_ptr<Material>& material);
	void SetProgramName(const std::string& programName);

private:
	//initializes the mesh 
	void StartMesh(const GLuint& drawType, const std::string& programName, const std::vector<GLuint>& elements);

	//turns raw vertex attributes into verticies vector
	void ConstructVerticiesFromRawData(const std::vector<glm::vec3>& positions = {}, const std::vector<glm::vec2>& textureCords = {}, const std::vector<glm::vec3>& normals = {});
	void SendVertexDataToGPU(const GLenum& usage = GL_STATIC_DRAW);

	//internal helpers for setting up a VAO and configuring it
	GLuint StartVAO();
	void EndVAO();
	void CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage);
	void EndBuffer(const GLenum& target);
	void CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset);

	//mesh vertices and indexing
	std::vector<Vertex> m_verticies;
	std::vector<GLuint> m_elements;

	//used when typical vertex structure is not followed and custom data is send in to gpu
	bool customVertex{ false };
	size_t m_vertexCount{0ull};
		

	//general mesh data
	GLuint
		m_VAO,
		m_drawType;

	std::string
		//shader link
		m_programName,
		//used for searching for a mesh in a hirearchy
		m_meshName;

	//a material that is made on mesh creation
	std::shared_ptr<Material> m_material;
};

#endif
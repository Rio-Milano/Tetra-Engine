#ifndef MESH_H
#define MESH_H

#include"../../Requirements/glmIncludes.h"

#include<glad/glad.h>
#include<vector>
#include<string>
#include<memory>
#include"Texture.h"

#define SHADER_LAYOUT_INDEX_POSITION		0
#define SHADER_LAYOUT_INDEX_TEXTURE_CORD	1
#define SHADER_LAYOUT_INDEX_COLOR			1
#define SHADER_LAYOUT_INDEX_NORMAL			2


struct RefractiveIndex
{
	static const float	Air,
						Water,
						Ice,
						Glass,
						Diamond;
};


struct ReflectionType
{
	static const int	Reflection,
						Refraction;
};



//simple vertex representation
//struct Vertex
//{
//	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
//	glm::vec2 textureCord{0.0f, 0.0f};
//	glm::vec3 normal{0.0f, 0.0f, 0.0f};
//};


//all materials encapsulated 
struct Material
{
	std::shared_ptr<Texture>
		m_diffuse,
		m_specular,
		m_emission;
	
	bool m_discardLowAlphaFragments{ false };
	bool m_blendingEnabled{false};
	bool m_mapToEnviroment{ false };
	int m_reflectionType{ ReflectionType::Reflection };//reflection
	float m_toRefractiveIndex = RefractiveIndex::Glass;

	//default values
	float m_ambientIntensity{ 0.1f };//amount of ambient light we can reflect
	float m_specularIntensity{ 3.f };//amount of specular lighting we can reflect

	glm::vec3 m_defaultDiffuseColor{ 1.0f, 1.0f, 1.0f };//default colors when no texture maps are avaliable
	glm::vec3 m_defaultSpecularColor{ 1.0f, 1.0f, 1.0f };
	
	float emissionRange{ 40.0f };//range on emission light source (treated as a light but only attenuation is applied)
};



enum class Attributes
{
	Positions	=		1 << 0,
	TexCords	=		1 << 1,
	Colors		=		1 << 2,
	Normals		=		1 << 3,
	Elements	=		1 << 4
};

inline Attributes operator |(const Attributes& a, const Attributes& b)
{
	return static_cast<Attributes>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline Attributes operator &(const Attributes& a, const Attributes& b)
{
	return static_cast<Attributes>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

//class for representing a collection of attributes opengl can use to display something
class Mesh
{
	friend class Renderer;
	friend class Plane;
	friend class Line;

public:
	//ctors/dtors
	Mesh();
	~Mesh();

	//for generating a user defined mesh
	void GenerateMesh
	(
		std::vector<glm::vec3>* positions =  nullptr,
		std::vector<glm::vec3>* normals = nullptr,
		std::vector<glm::vec2>* texCoords = nullptr,
		std::vector<glm::vec3>* colors = nullptr,
		std::vector<GLuint>* elements = nullptr,

		const GLuint& drawType = 1,
		const GLenum& usage = GL_STATIC_DRAW
	);


	//getters
	const std::string& GetMeshName() const { return m_meshName; };
	const std::shared_ptr<Material>& GetMaterial()const;
	const bool& GetFaceCullingFlag()const;

	//setters
	void SetMeshName(const std::string& meshName) { m_meshName = meshName; };
	void SetMaterial(const std::shared_ptr<Material>& material);
	void SetFaceCullingFlag(const bool& flag);
	
	void MakeAttributes(const Attributes& attributes);

	std::vector<glm::vec3>* m_colors{ nullptr };
	std::vector<glm::vec3>* m_positions{ nullptr };
	std::vector<glm::vec3>* m_normals{ nullptr };
	std::vector<glm::vec2>* m_texCoords{ nullptr };
	std::vector<GLuint>* m_elements{ nullptr };

private:
	//initializes the mesh 
	void StartMesh
	( 
		const GLuint& drawType,
		std::vector<glm::vec3>* positions,
		std::vector<glm::vec3>* normals,
		std::vector<glm::vec2>* texCoords,
		std::vector<glm::vec3>* colors,
		std::vector<GLuint>* elements
	);

	//turns raw vertex attributes into verticies vector
	void SendVertexDataToGPU(const GLenum& usage = GL_STATIC_DRAW);

	//internal helpers for setting up a VAO and configuring it
	GLuint StartVAO();
	void EndVAO();
	void CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage);
	void EndBuffer(const GLenum& target);
	void CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset);

	//used when typical vertex structure is not followed and custom data is send in to gpu
	bool customVertex{ false };
	size_t m_vertexCount{0ull};
	bool m_useCullingCCWBack{true};

	//general mesh data
	GLuint
		m_VAO,
		m_drawType;

	std::string
		//used for searching for a mesh in a hirearchy
		m_meshName;

	//a material that is made on mesh creation
	std::shared_ptr<Material> m_material;
};

#endif
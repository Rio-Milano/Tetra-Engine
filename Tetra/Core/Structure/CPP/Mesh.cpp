#include "../H/Mesh.h"
#include"../../Managers/H/TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include<iostream>

const float RefractiveIndex::Air{ 1.0f };
const float RefractiveIndex::Water{ 1.33f };
const float RefractiveIndex::Ice{ 1.309f };
const float RefractiveIndex::Glass{ 1.52f };
const float RefractiveIndex::Diamond{ 2.42f };

const int ReflectionType::Reflection{ 0 };
const int ReflectionType::Refraction{ 1 };


Mesh::Mesh()
{
	//create a material for mesh once its been made
	m_material = std::make_shared<Material>();
}

Mesh::~Mesh()
{
 	delete m_positions;
	delete m_normals;
	delete m_texCoords;
	delete m_elements;
	delete m_colors;
}


//manual mesh creation initializer
void Mesh::GenerateMesh
	(
		std::vector<glm::vec3>* positions,
		std::vector<glm::vec3>* normals,
		std::vector<glm::vec2>* texCoords,
		std::vector<glm::vec3>* colors,
		std::vector<GLuint>* elements,
		const GLuint& drawType, 
		const GLenum& usage
	)
{
	//the vertex attribute vector may have been set before call so use member as passed
	if (positions == nullptr) positions = m_positions;
	if (normals == nullptr) normals = m_normals;
	if (texCoords == nullptr) texCoords = m_texCoords;
	if (colors == nullptr) colors = m_colors;
	if (elements == nullptr) elements = m_elements;

	//assign attributes to mesh
	StartMesh(drawType, positions, normals, texCoords, colors, elements);
	
	SendVertexDataToGPU(usage);
}





const bool& Mesh::GetFaceCullingFlag() const
{
	return m_useCullingCCWBack;
}



void Mesh::SetFaceCullingFlag(const bool& flag)
{
	m_useCullingCCWBack = flag;
}



void Mesh::MakeAttributes(const Attributes& attributes)
{


	if (static_cast<unsigned int>(attributes & Attributes::Positions))m_positions = new std::vector<glm::vec3>;
	if (static_cast<unsigned int>(attributes & Attributes::TexCords))m_texCoords = new std::vector<glm::vec2>;
	if (static_cast<unsigned int>(attributes & Attributes::Colors))m_colors = new std::vector<glm::vec3>;
	if (static_cast<unsigned int>(attributes & Attributes::Normals))m_normals = new std::vector<glm::vec3>;
	if (static_cast<unsigned int>(attributes & Attributes::Elements))m_elements = new std::vector<unsigned int>;

}

void Mesh::StartMesh(const GLuint& drawType,
	std::vector<glm::vec3>* positions,
	std::vector<glm::vec3>* normals,
	std::vector<glm::vec2>* texCoords,
	std::vector<glm::vec3>* colors,
	std::vector<GLuint>* elements)
{
	//assign general mesh data

	//create a Vertex Array Object
	m_VAO = StartVAO();
	//set draw type so renderer knows what opengl call to make to draw it
	m_drawType = drawType;

	//assign buffers
	m_positions = positions;
	m_normals = normals;
	m_texCoords = texCoords;
	m_colors = colors;
	m_elements = elements;

}


void Mesh::SendVertexDataToGPU(const GLenum& usage)
{
	_ASSERT(m_positions != nullptr);

	//buffer sizes
	const size_t positionsBufferSize = m_positions->size() * sizeof(glm::vec3);
	
	//determine if using color buffer or texture cords buffer and adjust color buffer size accordingly
	size_t colorBufferSize = 0;
	if(m_texCoords != nullptr)
		colorBufferSize = m_positions->size() * sizeof(glm::vec2);
	else if (m_colors != nullptr)
		colorBufferSize = m_positions->size() * sizeof(glm::vec3);

	const size_t normalsBufferSize =  m_positions->size() * sizeof(glm::vec3);

	const size_t vertexBufferSizeSum =	positionsBufferSize + colorBufferSize + normalsBufferSize;

	//allocate buffer memory
	CreateBuffer(GL_ARRAY_BUFFER, vertexBufferSizeSum, nullptr , usage);


	//pass memory to buffer
	
	//position buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, positionsBufferSize, (void*)m_positions->data());
	if (m_texCoords) glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize, colorBufferSize, (void*)m_texCoords->data());
	else if (m_colors) glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize, colorBufferSize, (void*)m_colors->data());
	if (m_normals) glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize + colorBufferSize, normalsBufferSize, (void*)m_normals->data());


	//setup position stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	//setup color stream
	if(m_texCoords != nullptr)CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_TEXTURE_CORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)positionsBufferSize);
	else if(m_colors != nullptr)CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)positionsBufferSize);

	//setup normal stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(positionsBufferSize + colorBufferSize));

	//unbind buffer
	EndBuffer(GL_ARRAY_BUFFER);

	//set up elements
	if(m_elements) CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_elements->size(), m_elements->data(), GL_STATIC_DRAW);

	//unbind VAO then unbind elements
	EndVAO();

}



const std::shared_ptr<Material>& Mesh::GetMaterial()const
{
	return m_material;
}

void Mesh::SetMaterial(const std::shared_ptr<Material>& material)
{
	m_material = material;
}







GLuint Mesh::StartVAO()
{
	//define VAO
	GLuint VAO;
	//gen VA0
	glGenVertexArrays(1, &VAO);
	//bind VA0
	glBindVertexArray(VAO);
	//return VAO ID
	return VAO;
}

void Mesh::EndVAO()
{
	//unbind vao
	glBindVertexArray(0);
	//unbind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage)
{
	//use this function inside a VAO bind

	//define buffer id
	GLuint VBO;
	//gen buffer id
	glGenBuffers(1, &VBO);
	//bind to buffer target using id
	glBindBuffer(target, VBO);
	//send data into buffer
	glBufferData(target, size, data, usage);

}
void Mesh::EndBuffer(const GLenum& target)
{
	//unbind from target
	glBindBuffer(target, 0);
};

void Mesh::CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset)
{
	//setup index
	glEnableVertexAttribArray(index);
	//configure index
	glVertexAttribPointer(index, componentSize, componentType, normalized, stride, offset);
}
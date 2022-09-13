#include "Mesh.h"
#include"TextureManager.h"
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
		std::vector<GLuint>* elements,
		const GLuint& drawType, 
		const GLenum& usage,
		const std::string& programName
	)
{
	//upgrade to glBufferSubData at some point
	
	//assign attributes to mesh
	StartMesh(drawType, programName, positions, normals, texCoords, elements);



	//convert the raw verticies into the vertex representation
	//ConstructVerticiesFromRawData(positions, textureCords, normals);
	//load local data into the gpu
	
	SendVertexDataToGPU(usage);
}




const std::string& Mesh::GetProgramName()const
{
	return m_programName;
}

const bool& Mesh::GetFaceCullingFlag() const
{
	return m_useCullingCCWBack;
}


void Mesh::SetProgramName(const std::string& programName)
{
	m_programName = programName;
}

void Mesh::SetFaceCullingFlag(const bool& flag)
{
	m_useCullingCCWBack = flag;
}



void Mesh::StartMesh(const GLuint& drawType, const std::string& programName,
	std::vector<glm::vec3>* positions,
	std::vector<glm::vec3>* normals,
	std::vector<glm::vec2>* texCoords,
	std::vector<GLuint>* elements)
{
	//assign general mesh data

	//create a Vertex Array Object
	m_VAO = StartVAO();
	//set draw type so renderer knows what opengl call to make to draw it
	m_drawType = drawType;
	//assign it a shader program name
	m_programName = programName;
	//assign buffers
	m_positions = positions;
	m_normals = normals;
	m_texCoords = texCoords;
	m_elements = elements;

}


void Mesh::SendVertexDataToGPU(const GLenum& usage)
{
	_ASSERT(m_positions != nullptr);

	//buffer sizes
	const size_t positionsBufferSize = m_positions->size() * sizeof(glm::vec3);
	const size_t texCoordsBufferSize = m_positions->size() * sizeof(glm::vec2);
	const size_t normalsBufferSize =  m_positions->size() * sizeof(glm::vec3);
	const size_t vertexBufferSizeSum =	positionsBufferSize + texCoordsBufferSize + normalsBufferSize;

	//allocate buffer memory
	CreateBuffer(GL_ARRAY_BUFFER, vertexBufferSizeSum, nullptr , usage);


	//pass memory to buffer
	
	//position buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, positionsBufferSize, (void*)m_positions->data());
	if (m_texCoords) glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize, texCoordsBufferSize, (void*)m_texCoords->data());
	if (m_normals) glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize + texCoordsBufferSize, normalsBufferSize, (void*)m_normals->data());


	//setup position stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	//setup texture cord stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_TEXTURE_CORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)positionsBufferSize);

	//setup normal stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(positionsBufferSize + texCoordsBufferSize));

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
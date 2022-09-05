#include "Mesh.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include<iostream>

Mesh::Mesh()
{
	//create a material for mesh once its been made
	m_material = std::make_shared<Material>();
}


//manual mesh creation initializer
void Mesh::GenerateMesh
	(
		const std::vector<glm::vec3>& positions, 
		const std::vector<glm::vec3>& normals, 
		const std::vector<glm::vec2>& textureCords,
		const std::vector<GLuint>& elements, 
		const GLuint& drawType, 
		const GLenum& usage,
		const std::string& programName
	)
{
	//assign attributes to mesh
	StartMesh(drawType, programName, elements);
	//convert the raw verticies into the vertex representation
	ConstructVerticiesFromRawData(positions, textureCords, normals);
	//load local data into the gpu
	SendVertexDataToGPU(usage);
}



//mesh creator for assimp 
void Mesh::GenerateMesh
(
	const std::vector<Vertex>& verticies,
	const std::vector<GLuint>& elements,
	const GLuint& drawType,
	const GLenum& usage,
	const std::string& programName
)
{
	//assign attribues to mesh
	StartMesh(drawType, programName, elements);
	//assign verticies
	m_verticies = verticies;
	//load local data into gpu
	SendVertexDataToGPU(usage);
}



const std::string& Mesh::GetProgramName()const
{
	return m_programName;
}

void Mesh::SetProgramName(const std::string& programName)
{
	m_programName = programName;
}

void Mesh::StartMesh(const GLuint& drawType,const std::string& programName, const std::vector<GLuint>& elements)
{
	//assign general mesh data

	//create a Vertex Array Object
	m_VAO = StartVAO();
	//set draw type so renderer knows what opengl call to make to draw it
	m_drawType = drawType;
	//assign it a shader program name
	m_programName = programName;

	//assign vertex data
	m_elements = elements;
}

void Mesh::ConstructVerticiesFromRawData(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& textureCords, const std::vector<glm::vec3>& normals)
{
	//resize verticies vector for efficiency
	m_verticies.resize(positions.size());

	//we assume that if a vertex attribute vector has elements its size it identical to the other attribute vectors with size

	//loop every vertex
	for (size_t i = 0; i < positions.size(); i++)
	{
		//get current vertex
		Vertex& vertex = m_verticies[i];

		//if positions vector has positions then add position to current vertex
		if (positions.size())
			vertex.position = positions[i];

		//if texCords vector has cords then add cord to current vertex
		if (textureCords.size())
			vertex.textureCord = textureCords[i];
		#
		//if normals vector has normals then add normal to current vertex
		if (normals.size())
			vertex.normal = normals[i];
	}
}

void Mesh::SendVertexDataToGPU(const GLenum& usage)
{
	//this takes the data for the verticies array and sends it to a buffer then configures the stream data for gpu
	GLsizei vertexSize = sizeof(Vertex);

	//Send vertex data to gpu
	CreateBuffer(GL_ARRAY_BUFFER, m_verticies.size() * vertexSize, m_verticies.data(), usage);

	//setup position stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);

	//setup texture cord stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_TEXTURE_CORD, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex, textureCord));

	//setup normal stream
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)offsetof(Vertex, normal));

	//unbind buffer
	EndBuffer(GL_ARRAY_BUFFER);

	//set up elements
	CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_elements.size(), m_elements.data(), GL_STATIC_DRAW);

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
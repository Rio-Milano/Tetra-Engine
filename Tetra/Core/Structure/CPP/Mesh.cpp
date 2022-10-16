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
	delete m_tangents;
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
		const GLenum& usage,
		const bool& generateNormals,
		const bool& generateTangents
	)
{
	//the vertex attribute vector may have been set before call so use member as passed
	if (!positions) positions = m_positions;
	if (!normals) normals = m_normals;
	if (!texCoords) texCoords = m_texCoords;
	if (!colors) colors = m_colors;
	if (!elements) elements = m_elements;

	_ASSERT(positions);

	if (!normals && generateNormals)
	{
		//generate normals using elements
		if (elements && drawType == 0)
			GenerateNormalsFromElements(positions, elements, normals);
		else if (positions && drawType == 1)
			GenerateNormalsFromPositions(positions, normals);
	}

	//if normals and tex coods exist and tangents do not and we want to generate tangents
	if (normals && texCoords && !m_tangents && generateTangents)
	{
		//if using draw elements
		if (elements && drawType == 0)
			GenerateTangentsFromElements(positions, texCoords, elements);
		//if using draw arrays
		else if (positions && drawType == 1)
			GenerateTangentsFromPositions(positions, texCoords);
	}

	//assign attributes to mesh
	StartMesh(drawType, positions, normals, texCoords, colors, elements);
	
	SendVertexDataToGPU(usage);
}

void Mesh::CreateInstances(std::vector<glm::mat4>* offsets)
{
	//Check that offsets is not a null ptr
	_ASSERT(offsets != nullptr);

	//Check that the number of instances is valid
	_ASSERT(offsets->size() > 1);

	//Bind to the VAO of the mesh
	glBindVertexArray(m_VAO);

	//Alocate memory for the offsets and then push the offsets into the allocated memory
	CreateBuffer(GL_ARRAY_BUFFER, offsets->size() * sizeof(glm::mat4), offsets->data(), GL_STATIC_DRAW);
	
	//In the vertex shader we are taking in a mat4 which is split up into 4 vector 4's.

	const GLsizei vector4Size = static_cast<GLsizei>(sizeof(glm::vec4));//glm::vec4s are size 4 float * 4 instances
	const GLsizei mat4Size = static_cast<GLsizei>(sizeof(glm::mat4));//glm::mat4 are just sized to 4 glm::vec4s

	m_numberOfInstances = static_cast<GLsizei>(offsets->size());

	//ROW 1
	GLuint AttributeOffset = 0;
	unsigned long long MemoryOffset = 0;

	//ATTRIBUTE FOR ROW 1
		CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_OFFSET, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)MemoryOffset);
		glVertexAttribDivisor(SHADER_LAYOUT_INDEX_OFFSET, 1);//tels gpu to grab the next attribute every instance not every vertex

	//ATTRIBUTE FOR ROW 2
		AttributeOffset++;
		MemoryOffset += vector4Size;
		CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_OFFSET+AttributeOffset, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)MemoryOffset);
		glVertexAttribDivisor(SHADER_LAYOUT_INDEX_OFFSET+AttributeOffset, 1);

	//ATTRIBUTE FOR ROW 3
		AttributeOffset++;
		MemoryOffset += vector4Size;
		CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_OFFSET + AttributeOffset, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)MemoryOffset);
		glVertexAttribDivisor(SHADER_LAYOUT_INDEX_OFFSET + AttributeOffset, 1);

	//ATTRIBUTE FOR ROW 4
		AttributeOffset++;
		MemoryOffset += vector4Size;
		CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_OFFSET + AttributeOffset, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)MemoryOffset);
		glVertexAttribDivisor(SHADER_LAYOUT_INDEX_OFFSET + AttributeOffset, 1);

	//FINISHED
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
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
	//VALIDATE THAT POSITIONS ARE VALID
		_ASSERT(m_positions != nullptr);

	//buffer sizes

		//POSITIONS SIZE
			//We are guaranteed to have positions
			const size_t positionsBufferSize = m_positions->size() * sizeof(glm::vec3);

		/*
			UV / COLOURS SIZE
			determine if using color buffer or texture cords buffer and adjust color buffer size accordingly.
			We only use one not both
		*/
			size_t colorBufferSize = 0;
			if (m_texCoords != nullptr)	colorBufferSize = m_positions->size() * sizeof(glm::vec2);
			else
			{
				if (m_colors != nullptr) colorBufferSize = m_positions->size() * sizeof(glm::vec3);
			}

		//NORMALS SIZE
			size_t normalsBufferSize = 0;
			if(m_normals) normalsBufferSize = m_normals->size() * sizeof(glm::vec3);

		//TANGENTS SIZE
			size_t tangentBufferSize = 0;
			if (m_tangents) tangentBufferSize = m_tangents->size()* sizeof(glm::vec3);

		//SIZE OF BUFFERS
			const size_t vertexBufferSizeSum = positionsBufferSize + colorBufferSize + normalsBufferSize + tangentBufferSize;

	//ALLOCATE BUFFER MEMORY
		CreateBuffer(GL_ARRAY_BUFFER, vertexBufferSizeSum, nullptr , usage);


	//PASSING MEMORY TO BUFFER
		
		//POSITIONS
			glBufferSubData(GL_ARRAY_BUFFER, 0, positionsBufferSize, (void*)m_positions->data());

		//UV/COLOUR
			if (m_texCoords) glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize, colorBufferSize, (void*)m_texCoords->data());
			else { if (m_colors)glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize, colorBufferSize, (void*)m_colors->data()); };
	
		//NORMALS
			if (m_normals) glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize + colorBufferSize, normalsBufferSize, (void*)m_normals->data());

		//TANGENTS
			if (m_tangents) glBufferSubData(GL_ARRAY_BUFFER, positionsBufferSize + colorBufferSize + normalsBufferSize, tangentBufferSize, (void*)m_tangents->data());


	//VERTEX ATTRIBUTES

		//POSITIONS
			CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);	

		//UV/COLOUR
			if(m_texCoords != nullptr)	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_TEXTURE_CORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)positionsBufferSize);
			else { if (m_colors != nullptr)	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)positionsBufferSize); };

		//NORMALS
			if(m_normals)	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(positionsBufferSize + colorBufferSize));

		//TANGENTS
			if (m_tangents)	CreateVertexAttributePointer(GL_ARRAY_BUFFER, SHADER_LAYOUT_INDEX_TANGENT_OFFSET, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(positionsBufferSize + colorBufferSize + normalsBufferSize));

	//FINISHED WITH ARRAY BUFFER
		EndBuffer(GL_ARRAY_BUFFER);

	//ELEMENTS
		if(m_elements) CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_elements->size(), m_elements->data(), GL_STATIC_DRAW);

	//UNBIND FROM BUFFERS
		EndVAO();

}


const GLsizei Mesh::GetNumberOfInstances()const
{
	return m_numberOfInstances;
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

void Mesh::GenerateNormalsFromPositions(std::vector<glm::vec3>* positions, std::vector<glm::vec3>*& normals)
{
	//check that positions are sized to a multiple of 3
	const double positionsOver3 = static_cast<double>(positions->size()) / 3.0;
	_ASSERT(positionsOver3 - std::floor(positionsOver3) == 0.0);

	normals = new std::vector<glm::vec3>();
	normals->resize(positions->size());

	for (size_t i = 0; i < positions->size(); i += 3)
	{
		const glm::vec3&
			A = (*positions)[i],
			B = (*positions)[i+1],
			C = (*positions)[i+2];

		glm::vec3
			AB = B - A,
			AC = C - A,
			ABcrossACnormalized = glm::normalize(glm::cross(AB, AC));

		(*normals)[i] = ABcrossACnormalized;
		(*normals)[i+1] = ABcrossACnormalized;
		(*normals)[i+2] = ABcrossACnormalized;
	}
	std::cout << "Generated Normals from positions!\n";
}

void Mesh::GenerateNormalsFromElements(std::vector<glm::vec3>* positions, std::vector<GLuint>* elements, std::vector<glm::vec3>*& normals)
{
	//check that elements are sized to a multiple of 3
	const double elementsOver3 = static_cast<double>(elements->size()) / 3.0;
	_ASSERT(elementsOver3 - std::floor(elementsOver3) == 0.0);

	normals = new std::vector<glm::vec3>;
	normals->resize(positions->size());

	for (size_t i = 0; i < elements->size(); i += 3)
	{
		const glm::vec3& 
			A = (*positions)[(*elements)[i]],
			B = (*positions)[(*elements)[i+1]],
			C = (*positions)[(*elements)[i+2]];

		glm::vec3 
			AB = B - A,
			AC = C - A,
			ABcrossAC = glm::cross(AB, AC);

		(*normals)[(*elements)[i]] += ABcrossAC;
		(*normals)[(*elements)[i+1]] += ABcrossAC;
		(*normals)[(*elements)[i+2]] += ABcrossAC;

	}

	//renormalize
	for (size_t i = 0; i < normals->size(); i++)
		(*normals)[i] = glm::normalize((*normals)[i]);

}

void Mesh::GenerateTangentsFromElements(const std::vector<glm::vec3>* positions, const std::vector<glm::vec2>* texCoords, const std::vector<GLuint>* elements)
{
	//check that the size of positions is a multiple of 3
#if _DEBUG
	float positionsOver3 = static_cast<float>(positions->size()) / 3.0f;
	_ASSERT(positionsOver3 - std::floor(positionsOver3) == 0);
#endif

	//allocate tangents memory
	m_tangents = new std::vector<glm::vec3>;
	m_tangents->resize(positions->size());

	//loop through triangle primitives
	for (size_t i = 0; i < elements->size(); i+=3)
	{
		//get the 3 vertex positions
		const glm::vec3&
			Apos = positions->at(elements->at(i + 0)),
			Bpos = positions->at(elements->at(i + 1)),
			Cpos = positions->at(elements->at(i + 2));
		
		//get the 3 vertex texCoords
		const glm::vec2&
			Atex = texCoords->at(elements->at(i + 0)),
			Btex = texCoords->at(elements->at(i + 1)),
			Ctex = texCoords->at(elements->at(i + 2));

		glm::vec3 
			edge_1 = Bpos - Apos,
			edge_2 = Cpos - Apos;

		float
			deltaU1 = Btex.x - Atex.x,
			deltaV1 = Btex.y - Atex.y,

			deltaU2 = Ctex.x - Atex.x,
			deltaV2 = Ctex.y - Atex.y;

		float UVmatrixDeterminant = deltaU1 * deltaV2 - deltaV1 * deltaU2;
		float cofactor = 1.0f / UVmatrixDeterminant;

		glm::vec3 tangent(0.0f);

		tangent.x = cofactor * (deltaV2 * edge_1.x + (-deltaV1) * edge_2.x);
		tangent.y = cofactor * (deltaV2 * edge_1.y + (-deltaV1) * edge_2.y);
		tangent.z = cofactor * (deltaV2 * edge_1.z + (-deltaV1) * edge_2.z);

		m_tangents->at(elements->at(i + 0)) += tangent;
		m_tangents->at(elements->at(i + 1)) += tangent;
		m_tangents->at(elements->at(i + 2)) += tangent;
	}



}

void Mesh::GenerateTangentsFromPositions(const std::vector<glm::vec3>* positions, const std::vector<glm::vec2>* texCoords)
{
	//check that the size of positions is a multiple of 3
#if _DEBUG
	float positionsOver3 = static_cast<float>(positions->size()) / 3.0f;
	_ASSERT(positionsOver3 - std::floor(positionsOver3) == 0);
#endif

	//allocate tangents memory
	m_tangents = new std::vector<glm::vec3>;
	m_tangents->resize(positions->size());

	//loop through triangle primitives
	for (size_t i = 0; i < positions->size(); i += 3)
	{
		//get the 3 vertex positions
		const glm::vec3&
			Apos = positions->at(i + 0),
			Bpos = positions->at(i + 1),
			Cpos = positions->at(i + 2);

		//get the 3 vertex texCoords
		const glm::vec2&
			Atex = texCoords->at(i + 0),
			Btex = texCoords->at(i + 1),
			Ctex = texCoords->at(i + 2);

		glm::vec3
			edge_1 = Bpos - Apos,
			edge_2 = Cpos - Apos;

		float
			deltaU1 = Btex.x - Atex.x,
			deltaV1 = Btex.y - Atex.y,

			deltaU2 = Ctex.x - Atex.x,
			deltaV2 = Ctex.y - Atex.y;

		float UVmatrixDeterminant = deltaU1 * deltaV2 - deltaU2 * deltaV1;
		float cofactor = 1.0f / UVmatrixDeterminant;

		glm::vec3 tangent(0.0f);

		tangent.x = cofactor * (deltaV2 * edge_1.x  - deltaV1 * edge_2.x);
		tangent.y = cofactor * (deltaV2 * edge_1.y - deltaV1 * edge_2.y);
		tangent.z = cofactor * (deltaV2 * edge_1.z - deltaV1 * edge_2.z);


		m_tangents->at(i + 0) = tangent;
		m_tangents->at(i + 1) = tangent;
		m_tangents->at(i + 2) = tangent;
	}

}

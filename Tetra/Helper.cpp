#include"Helper.h"


GLuint Helper_::Mesh_::StartVAO()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	return VAO;
}

void Helper_::Mesh_::EndVAO()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Helper_::Mesh_::CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage)
{
	//use this function inside a VAO bind

	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(target, VBO);
	glBufferData(target, size, data, usage);

};

void Helper_::Mesh_::CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, componentSize, componentType, normalized, stride, offset);

	glBindBuffer(target, 0);
}


void Helper_::Mesh_::LoadTexture(const std::string& textureFileLocation, TextureAttributes& textureAttributes)
{

	textureAttributes.texturePtr = stbi_load(textureFileLocation.c_str(), &textureAttributes.width, &textureAttributes.height, &textureAttributes.channels, 0);

	if (textureAttributes.texturePtr == nullptr)
	{
		std::cout << "Failed to Load Texture :" << textureFileLocation << std::endl;
		return;
	}
}

void Helper_::Mesh_::GenerateTextureBuffer(GLuint& textureBuffer, TextureAttributes& textureAttributes)
{
	int&
		texWidth{ textureAttributes.width },
		texHeight{ textureAttributes.height };

	unsigned char*& texturePtr{ textureAttributes.texturePtr };

	glGenTextures(1, &textureBuffer);

	glBindTexture(GL_TEXTURE_2D, textureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texturePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Helper_::Mesh_::GenerateMesh(Mesh& mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec2>& textureCords, const std::string& textureLocation, const GLuint& drawType, const std::vector<GLuint>& elements)
{
	mesh.m_VAO = StartVAO();
	mesh.m_numberOfElements = elements.size();
	mesh.m_drawType = drawType;
	mesh.m_triangleCount = static_cast<GLuint>(positions.size());

	//Positions
	CreateBuffer(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	//Texture
	TextureAttributes textureAttribs;
	LoadTexture(textureLocation, textureAttribs);
	GenerateTextureBuffer(mesh.m_textureID, textureAttribs);

	CreateBuffer(GL_ARRAY_BUFFER, textureCords.size() * sizeof(glm::vec2), textureCords.data(), GL_STATIC_DRAW);
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, 1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), (void*)elements.data(), GL_STATIC_DRAW);

	EndVAO();
}


void Helper_::Status_::DisplayUsefulInfo()
{
	int maxAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
	std::cout << "The maximun number of vertex Attributes supported by your hardware is :" << maxAttribs << std::endl;
}


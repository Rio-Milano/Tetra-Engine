#include"Helper.h"

const GLuint Helper_::Shaders_::CreateShader(const GLenum& shaderType, const std::string& shaderFileLocation)
{
	GLuint shaderID;
	shaderID = glCreateShader(shaderType);

	std::fstream fileStream(shaderFileLocation, std::fstream::in);
	if (!fileStream.is_open())
	{
		std::cout << std::string("Shader file at :" + shaderFileLocation + " could not be opened.\n");
		return 0;
	}
	std::string content((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));//string from file
	fileStream.close();

	const char* contentPtr{ content.c_str() };
	glShaderSource(shaderID, 1, &contentPtr, 0);

	glCompileShader(shaderID);

	int successOfCompilingShader;
	char errorMessages[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &successOfCompilingShader);

	std::string shaderTypeName{ shaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader" };

	if (!successOfCompilingShader)
	{
		glGetShaderInfoLog(shaderID, 512, 0, errorMessages);

		std::cout << std::string("Error Compiling " + shaderTypeName + ", error :" + errorMessages + "\n");
		return 0;
	}

	std::cout << shaderTypeName << " Created!\n";
	return shaderID;
};


const GLuint Helper_::Shaders_::CreateProgram(const GLuint& vertexShaderID, const GLuint& fragmentShaderID)
{
	GLuint programID;
	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	int linkStatus;
	char linkErrors[512];

	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus)
	{
		glGetProgramInfoLog(programID, 512, 0, linkErrors);
		std::cout << "Program failed to link shaders, error :" << linkErrors << std::endl;
		return 0;
	}

	std::cout << "Program Created!\n";
	return programID;

}

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

void Helper_::Mesh_::GenerateMesh(Mesh& mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec2>& textureCords, std::vector<GLuint>& elements, const std::string& textureLocation)
{
	mesh.VAO = StartVAO();
	mesh.numberOfElements = elements.size();

	//Positions
	CreateBuffer(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	//Texture
	TextureAttributes textureAttribs;
	LoadTexture(textureLocation, textureAttribs);
	GenerateTextureBuffer(mesh.textureID, textureAttribs);

	CreateBuffer(GL_ARRAY_BUFFER, textureCords.size() * sizeof(glm::vec2), textureCords.data(), GL_STATIC_DRAW);
	CreateVertexAttributePointer(GL_ARRAY_BUFFER, 1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);

	EndVAO();
}

void Helper_::Mesh_::DrawMesh(const Mesh& mesh)
{
	glBindVertexArray(mesh.VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.textureID);

	glDrawElements(GL_TRIANGLES, mesh.numberOfElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

}

void Helper_::Status_::DisplayUsefulInfo()
{
	int maxAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
	std::cout << "The maximun number of vertex Attributes supported by your hardware is :" << maxAttribs << std::endl;
}


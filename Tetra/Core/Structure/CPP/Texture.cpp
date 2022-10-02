#include "../H/Texture.h"

#include<iostream>

void Texture::LoadTexture(const std::string& textureFileLocation, const bool& flipOnLoad)
{
	stbi_set_flip_vertically_on_load(flipOnLoad);

	//load the texture from disk
	m_textureAttributes.texturePtr = stbi_load(textureFileLocation.c_str(), &m_textureAttributes.width, &m_textureAttributes.height, &m_textureAttributes.channels, 0);

	//if load failed then report error
	if (m_textureAttributes.texturePtr == nullptr)
	{
		_ASSERT(0);
		std::cout << "Failed to Load Texture :" << textureFileLocation << std::endl;
		return;
	}
	else
	{
		//if texture loaded then set load flag to true
		m_textureAttributes.validTexture = true;
	}
}

//getters
Texture::TextureAttributes& Texture::GetTextureAttributes()
{
	return m_textureAttributes;
}

//loaders
void Texture::InitializeTexture(const std::string& textureFileLocation, const bool& flipOnLoad, const bool& linearSpace)
{
	LoadTexture(textureFileLocation, flipOnLoad);
	GenerateTextureBuffer(textureFileLocation, linearSpace);
}

void Texture::GenerateTextureBuffer(const std::string& textureFileLocation, const bool& linearSpace)
{
	//get quick access to attributes of texture
	int&
		texWidth{ m_textureAttributes.width },
		texHeight{ m_textureAttributes.height };

	unsigned char*& texturePtr{ m_textureAttributes.texturePtr };

	//gen texture buffer to id in attributes
	glGenTextures(1, &m_textureAttributes.textureID);
	//bind to texture 2d with id
	glBindTexture(GL_TEXTURE_2D, m_textureAttributes.textureID);
	//set wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//set filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//decode file type
	
	GLint imageType_EN_A = 0;
	GLint imageType_EN_B = 0;
	std::string extension = textureFileLocation.substr(textureFileLocation.find_last_of('.') + 1, textureFileLocation.size() - 1);
	if (extension == "png")
	{
		if (linearSpace)
			imageType_EN_A = GL_SRGB_ALPHA;
		else
			imageType_EN_A = GL_RGBA;

		imageType_EN_B = GL_RGBA;
	}
	else if (extension == "jpg")
	{
		if (linearSpace)
			imageType_EN_A = GL_SRGB;
		else
			imageType_EN_A = GL_RGB;
		
		imageType_EN_B = GL_RGB;
	}

	//send texture to gpu texture buffer
	glTexImage2D(GL_TEXTURE_2D, 0, imageType_EN_A, texWidth, texHeight, 0, imageType_EN_B, GL_UNSIGNED_BYTE, texturePtr);
	//generate texture layers
	glGenerateMipmap(GL_TEXTURE_2D);
	//unbind buffer target
	glBindTexture(GL_TEXTURE_2D, 0);
	//delete texture from primary memory
	stbi_image_free(m_textureAttributes.texturePtr);
}
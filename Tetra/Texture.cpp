#include "Texture.h"

#include<iostream>

void Texture::LoadTexture(const std::string& textureFileLocation)
{

	m_textureAttributes.texturePtr = stbi_load(textureFileLocation.c_str(), &m_textureAttributes.width, &m_textureAttributes.height, &m_textureAttributes.channels, 0);

	if (m_textureAttributes.texturePtr == nullptr)
	{
		std::cout << "Failed to Load Texture :" << textureFileLocation << std::endl;
		return;
	}
}

const Texture::TextureAttributes& Texture::GetTextureAttributes() const
{
	return m_textureAttributes;
}

void Texture::InitializeTexture(const std::string& textureFileLocation)
{
	LoadTexture(textureFileLocation);
	GenerateTextureBuffer();
}

void Texture::GenerateTextureBuffer()
{
	int&
		texWidth{ m_textureAttributes.width },
		texHeight{ m_textureAttributes.height };

	unsigned char*& texturePtr{ m_textureAttributes.texturePtr };

	glGenTextures(1, &m_textureAttributes.textureID);

	glBindTexture(GL_TEXTURE_2D, m_textureAttributes.textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texturePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(m_textureAttributes.texturePtr);
}
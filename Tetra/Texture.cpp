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
	else
	{
		m_textureAttributes.validTexture = true;
	}
}

const Texture::TextureAttributes& Texture::GetTextureAttributes() const
{
	return m_textureAttributes;
}

void Texture::InitializeTexture(const std::string& textureFileLocation, const ImageType& imageType)
{
	LoadTexture(textureFileLocation);
	GenerateTextureBuffer(imageType);
}

void Texture::GenerateTextureBuffer(const ImageType& imageType)
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

	GLenum imageType_EN;

	switch (imageType)
	{
	case ImageType::png:
		imageType_EN = GL_RGBA;
		break;
	case ImageType::jpg:
		imageType_EN = GL_RGB;
		break;
	default:
		std::cout << "Image type not recognized\n";
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, imageType_EN, GL_UNSIGNED_BYTE, texturePtr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(m_textureAttributes.texturePtr);
}
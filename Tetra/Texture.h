#ifndef TEXTURE_H
#define TEXTURE_H

#include<STB_ImageLoader/stb_image.h>
#include<glad/glad.h>
#include<string>


class Texture
{
public:
	struct TextureAttributes
	{
		int width{ 0 }, height{ 0 }, channels{ 0 };
		unsigned char* texturePtr{ nullptr };

		GLuint textureID{0};
		bool validTexture{false};
	};
	
	const TextureAttributes& GetTextureAttributes()const;

	void InitializeTexture(const std::string& textureFileLocation);

private:
	void LoadTexture(const std::string& textureFileLocation);
	void GenerateTextureBuffer(const std::string& textureFileLocation);

	TextureAttributes m_textureAttributes;
};

#endif
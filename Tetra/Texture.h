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
		int width{ 0 }, height{ 0 }, channels{ 0 };//this values are persistant
		unsigned char* texturePtr{ nullptr };//texture is deleted once loaded onto the gpu

		GLuint textureID{0};//texture id on gpu
		bool validTexture{false};//did texture load successfully
	};
	
	//getters
	const TextureAttributes& GetTextureAttributes()const;

	//loaders
	void InitializeTexture(const std::string& textureFileLocation, const bool& flipOnLoad = false);

private:
	//internal helpers
	void LoadTexture(const std::string& textureFileLocation, const bool& flipOnLoad);
	void GenerateTextureBuffer(const std::string& textureFileLocation);

	//members
	TextureAttributes m_textureAttributes;
};

#endif
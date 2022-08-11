#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include"Texture.h"

#include<map>
#include<string>

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>


class TextureManager
{
public:

	static TextureManager& GetInstance();

	void AddTexture(const std::string& textureName, const Texture& textureObj);
	const Texture& GetTexture(const std::string& textureName);

private:
	TextureManager() = default;
	~TextureManager() = default;

	std::map<std::string, Texture> m_textureName_to_TextureObj;
};


#endif
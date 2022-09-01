#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include<unordered_map>
#include<string>
#include<memory>

class Texture;

class TextureManager
{
public:

	static TextureManager& GetInstance();

	void AddTexture(const std::string& textureName, const std::shared_ptr<Texture>& textureObj);
	const std::shared_ptr<Texture>& GetTexture(const std::string& textureName);

private:
	TextureManager() = default;
	~TextureManager() = default;

	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureName_to_TextureObj;
};


#endif
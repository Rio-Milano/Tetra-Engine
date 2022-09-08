#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include<unordered_map>
#include<string>
#include<memory>

class CubeMap;
class Texture;

class TextureManager
{
public:

	static TextureManager& GetInstance();

	const std::shared_ptr<Texture>& GetTexture(const std::string& textureName);
	const std::shared_ptr<CubeMap>& GetSkyBoxCubeMap()const;

	void AddTexture(const std::string& textureName, const std::shared_ptr<Texture>& textureObj);
	void SetActiveSkyBoxCubeMap(const std::shared_ptr<CubeMap>& cubeMap);
private:
	TextureManager() = default;
	~TextureManager() = default;

	std::shared_ptr<CubeMap> m_activeSkyBoxCubeMap = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureName_to_TextureObj;
};


#endif
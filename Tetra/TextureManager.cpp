#include "TextureManager.h"

#include<iostream>

TextureManager& TextureManager::GetInstance()
{
    static TextureManager instance;
    return instance;
}

void TextureManager::AddTexture(const std::string& textureName, const std::shared_ptr<Texture>& textureObj)
{
    if (m_textureName_to_TextureObj.find(textureName) != m_textureName_to_TextureObj.end())
        std::cout << "ERROR ->Texture :" << textureName << " already exists!\n";
    else
        m_textureName_to_TextureObj[textureName] = textureObj;
}

const std::shared_ptr<Texture>& TextureManager::GetTexture(const std::string& textureName)
{
    static std::shared_ptr<Texture> fallBack = nullptr;

    if (m_textureName_to_TextureObj.find(textureName) == m_textureName_to_TextureObj.end())
    {
        return fallBack;
    }
    else
        return m_textureName_to_TextureObj[textureName];
 }

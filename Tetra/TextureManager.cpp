#include "TextureManager.h"

#include<iostream>

TextureManager& TextureManager::GetInstance()
{
    static TextureManager instance;
    return instance;
}

void TextureManager::AddTexture(const std::string& textureName, const Texture& textureObj)
{
    if (m_textureName_to_TextureObj.find(textureName) != m_textureName_to_TextureObj.end())
        std::cout << "ERROR ->Texture :" << textureName << " already exists!\n";
    else
        m_textureName_to_TextureObj[textureName] = textureObj;
}

const Texture& TextureManager::GetTexture(const std::string& textureName)
{
    static Texture fallBack;

    if (m_textureName_to_TextureObj.find(textureName) == m_textureName_to_TextureObj.end())
    {
        std::cout << "ERROR-> Texture :" << textureName << "does not exist!\n";
        return fallBack;
    }
    else
        return m_textureName_to_TextureObj[textureName];
 }

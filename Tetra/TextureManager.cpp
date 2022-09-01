#include "TextureManager.h"

#include<iostream>

#include"Texture.h"

TextureManager& TextureManager::GetInstance()
{
    //singleton instnce class
    static TextureManager instance;
    return instance;
}

void TextureManager::AddTexture(const std::string& textureName, const std::shared_ptr<Texture>& textureObj)
{
    //if texture name is already a key in the hash map then report error
    if (m_textureName_to_TextureObj.find(textureName) != m_textureName_to_TextureObj.end())
        std::cout << "ERROR ->Texture :" << textureName << " already exists!\n";
    else
        //add key value pair
        m_textureName_to_TextureObj[textureName] = textureObj;
}

const std::shared_ptr<Texture>& TextureManager::GetTexture(const std::string& textureName)
{
    //static used for referancing
    static std::shared_ptr<Texture> fallBack = nullptr;

    //if teture name value not found then return null else return value
    if (m_textureName_to_TextureObj.find(textureName) == m_textureName_to_TextureObj.end())
    {
        return fallBack;
    }
    else
        return m_textureName_to_TextureObj[textureName];
 }

#ifndef CUBEMAP_H
#define CUBEMAP_H

#include<glad/glad.h>
#include<vector>
#include<string>
#include<STB_ImageLoader/stb_image.h>
#include<crtdbg.h>

class CubeMap
{
public:
	CubeMap() = default;
	~CubeMap() = default;
	
	const GLuint& GetCubeMapTextureID()const
	{
		return m_cubeMapTextureID;
	};

	/*
	Pass the face locations in this order:
	
	GL_TEXTURE_CUBE_MAP_POSITIVE_X 
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X 
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y 
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z 
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	*/
	void InitializeCubeMap(const std::vector<std::string>& faceLocations)
	{
		//if not required face dirs trigger break point
		_ASSERT(faceLocations.size() == 6ull);

		//gen a buffer for cube map
		glGenTextures(1, &m_cubeMapTextureID);
		//bind to cube map
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTextureID);

		int width = 0, height = 0, channels = 0;
		unsigned char* textureData = nullptr;

		stbi_set_flip_vertically_on_load(false);

		for (size_t i = 0; i < faceLocations.size(); i++)
		{
			textureData = stbi_load(faceLocations[i].c_str(), &width, &height, &channels, 0);
			_ASSERT(textureData != nullptr);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(i), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			stbi_image_free(textureData);
		}

		//set sampling config 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//set wrapping config (ensures that ray hits face on cube map)
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//unbind from target
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

private:
	GLuint m_cubeMapTextureID;
};

#endif

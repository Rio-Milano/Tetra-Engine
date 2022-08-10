#ifndef HELPER_H
#define HELPER_H

#include"STB_ImageLoader/stb_image.h"
#include"glmIncludes.h"
#include"Entity.h"

#include<glad/glad.h>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>

namespace Helper_
{

	namespace Mesh_
	{

		struct TextureAttributes
		{
			~TextureAttributes() { stbi_image_free(texturePtr); };

			int width{ 0 }, height{ 0 }, channels{ 0 };
			unsigned char* texturePtr{ nullptr };
		};

		GLuint StartVAO();
		void EndVAO();

		void CreateBuffer(const GLenum& target, const GLsizeiptr& size, void* data, const GLenum& usage);
		void CreateVertexAttributePointer(const GLenum& target, const GLuint& index, const GLint& componentSize, const GLenum& componentType, const GLenum& normalized, const GLsizei& stride, void* offset);

		void LoadTexture(const std::string& textureFileLocation, TextureAttributes& textureAttributes);
		void GenerateTextureBuffer(GLuint& textureBuffer, TextureAttributes& textureAttributes);

		void GenerateMesh(Mesh& mesh, std::vector<glm::vec3>& positions, std::vector<glm::vec2>& textureCords, const std::string& textureLocation, const GLuint& drawType, const std::vector<GLuint>& elements = std::vector<GLuint>{});
	};

	namespace Status_
	{
		void DisplayUsefulInfo();
	};

};

#endif
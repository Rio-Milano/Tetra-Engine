#include"MeshManager.h"
#include<crtdbg.h>
#include<iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

MeshManager & MeshManager::GetInstance()
{
	static MeshManager instance;
	return instance;
}



const std::shared_ptr<Mesh>& MeshManager::GetMesh(const std::string& meshName)
{
	static std::shared_ptr<Mesh> fallBack = std::make_shared<Mesh>();

	if (m_meshName_mesh_map.find(meshName) == m_meshName_mesh_map.end())
	{
		std::cout << "ERROR-> Texture :" << meshName << "does not exist!\n";
		return fallBack;
	}
	else
		return m_meshName_mesh_map[meshName];
}

std::shared_ptr<ModelNode> MeshManager::LoadModel(const std::string& modelPath)
{
	const char* path = modelPath.c_str();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "Failed to load model, path :" << modelPath << std::endl;
		std::cout << "Error (Assimp) :" << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	/*
	Data/Models/CarCar.obj
	Data/Models/
	*/
	std::string modelDirectory = modelPath.substr(0, modelPath.find_last_of('/'));


	return PullAssimpMeshFromNode(scene->mRootNode, scene);
}

std::shared_ptr<ModelNode> MeshManager::PullAssimpMeshFromNode(aiNode* node, const aiScene* scene, std::shared_ptr<ModelNode> modelNode)
{
	if (modelNode == nullptr)
	{
		aiMatrix4x4& transform = node->mTransformation;

		glm::mat4 glmMat4
		{
			static_cast<float>(transform.a1),static_cast<float>(transform.a2),static_cast<float>(transform.a3),static_cast<float>(transform.a4),
			static_cast<float>(transform.b1),static_cast<float>(transform.b2),static_cast<float>(transform.b3),static_cast<float>(transform.b4),
			static_cast<float>(transform.c1),static_cast<float>(transform.c2),static_cast<float>(transform.c3),static_cast<float>(transform.c4),
			static_cast<float>(transform.d1),static_cast<float>(transform.d2),static_cast<float>(transform.d3),static_cast<float>(transform.d4),
		};
		modelNode = std::make_shared<ModelNode>(glmMat4, std::string(node->mName.C_Str()));
	}

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::shared_ptr<Mesh> newMesh = ConstructMeshFromAssimpMesh(mesh, scene);
		modelNode->AddMesh(newMesh);
	}

	for (int j = 0; j < node->mNumChildren; j++)
	{
		aiMatrix4x4& transform = node->mChildren[j]->mTransformation;

		glm::mat4 glmMat4
		{
			static_cast<float>(transform.a1),static_cast<float>(transform.a2),static_cast<float>(transform.a3),static_cast<float>(transform.a4),
			static_cast<float>(transform.b1),static_cast<float>(transform.b2),static_cast<float>(transform.b3),static_cast<float>(transform.b4),
			static_cast<float>(transform.c1),static_cast<float>(transform.c2),static_cast<float>(transform.c3),static_cast<float>(transform.c4),
			static_cast<float>(transform.d1),static_cast<float>(transform.d2),static_cast<float>(transform.d3),static_cast<float>(transform.d4),
		};

		std::shared_ptr<ModelNode> childModelNode = std::make_shared<ModelNode>(glmMat4, std::string(node->mChildren[j]->mName.C_Str()));
		PullAssimpMeshFromNode(node->mChildren[j], scene, childModelNode);
		modelNode->AddChild(childModelNode);
	}

	return modelNode;
}

#define TRIANGLE_PRIMITIVE_SIZE 3
std::shared_ptr<Mesh> MeshManager::ConstructMeshFromAssimpMesh(aiMesh* assimpMesh, const aiScene* scene)
{
	std::vector<Vertex> verticies;
	verticies.resize(assimpMesh->mNumVertices);

	std::vector<GLuint> elements;
	//??elements.resize(TRIANGLE_PRIMITIVE_SIZE * assimpMesh->mNumFaces);


	//convert the vertex data from assimp structure to my structure
	for (size_t i = 0; i < assimpMesh->mNumVertices; i++)
	{
		Vertex& vertex = verticies[i];

		const aiVector3D& assimpVertexPosition = assimpMesh->mVertices[i];
		vertex.position.x = assimpVertexPosition.x;
		vertex.position.y = assimpVertexPosition.y;
		vertex.position.z = assimpVertexPosition.z;
		//else leave position
		
			const aiVector3D& assimpVertexNormal = assimpMesh->mNormals[i];
			vertex.normal.x = assimpVertexNormal.x;
			vertex.normal.y = assimpVertexNormal.y;
			vertex.normal.z = assimpVertexNormal.z;
		//else leave normal


		vertex.textureCord.x = assimpMesh->mTextureCoords[0][i].x;
		vertex.textureCord.y = assimpMesh->mTextureCoords[0][i].y;

	}

	if (assimpMesh->HasFaces())
	{
		//??size_t elementIndex = 0;
		for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
		{
			const aiFace& face = assimpMesh->mFaces[i];

			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				elements.emplace_back(face.mIndices[j]);
			}
		}
	}

	std::shared_ptr<Texture> diffuseMap = nullptr;
	std::shared_ptr<Texture> specularMap = nullptr;
	std::shared_ptr<Texture> emissionMap = nullptr;

	if (scene->HasMaterials())
	{
		if (assimpMesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];

			//load diffuse map
			diffuseMap = LoadMaterialFromAssimpMesh(material, aiTextureType_DIFFUSE);

			//load specular map
			specularMap = LoadMaterialFromAssimpMesh(material, aiTextureType_SPECULAR);

			//emission map??
			emissionMap = LoadMaterialFromAssimpMesh(material, aiTextureType_EMISSIVE);

		}
	}

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	mesh->GenerateMesh(verticies, elements, diffuseMap, specularMap, emissionMap, 0);

	return mesh;
}

std::shared_ptr<Texture> MeshManager::LoadMaterialFromAssimpMesh(aiMaterial* material, aiTextureType type)
{
	//for now we are only loading 1 material per type eventhough there may be more
	static const size_t numberOfMaterialsPerType = 1;

	if (material->GetTextureCount(type) >= numberOfMaterialsPerType)
	{
		std::vector<std::shared_ptr<Texture>> textures;
		textures.resize(numberOfMaterialsPerType);

		for (size_t i = 0; i < numberOfMaterialsPerType; i++)
		{			
			aiString fileLocation;
			material->GetTexture(type, i, &fileLocation);

			if (TextureManager.GetTexture("Data/Models/backpack/" + std::string(fileLocation.C_Str())) == nullptr)
			{
				textures[i] = std::make_shared<Texture>();
				textures[i]->InitializeTexture("Data/Models/backpack/" + std::string(fileLocation.C_Str()));
				TextureManager.AddTexture("Data/Models/backpack/" + std::string(fileLocation.C_Str()), textures[i]);
			}
			textures[i] = TextureManager.GetTexture("Data/Models/backpack/" + std::string(fileLocation.C_Str()));

		}

		return textures[0];
	}
	return nullptr;
}

void MeshManager::AddMesh(const std::string& meshName, const std::shared_ptr<Mesh>& mesh)
{
	if (m_meshName_mesh_map.find(meshName) == m_meshName_mesh_map.end())
		m_meshName_mesh_map[meshName] = mesh;
	else
		_ASSERT(false);//mesh already exists
}
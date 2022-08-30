#include<crtdbg.h>
#include<iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include"MeshManager.h"
#include"Mesh.h"
#include"Model.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

//singleton function
MeshManager & MeshManager::GetInstance()
{
	static MeshManager instance;
	return instance;
}



const std::shared_ptr<Mesh>& MeshManager::GetMesh(const std::string& meshName)
{
	//define an empty pointer so we can referance it if needed
	static std::shared_ptr<Mesh> fallBack = nullptr;

	//search the data store for a 'meshName'
	if (m_meshName_mesh_map.find(meshName) == m_meshName_mesh_map.end())
	{
		return fallBack;
	}
	else
	{
		//data exists so return it
		return m_meshName_mesh_map[meshName];
	}
}

//recieve a model location
std::shared_ptr<Model> MeshManager::LoadModel(const std::string& modelPath)
{
	//get the path as a const char *
	const char* path = modelPath.c_str();

	//define an importer object to load the model
	Assimp::Importer importer;

	//tell importer to read path and force triangulation then hold the resulting scene ptr
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	//if scene is empty or scene failed some point or root node of scene is empty then report error
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "Failed to load model, path :" << modelPath << std::endl;
		std::cout << "Error (Assimp) :" << importer.GetErrorString() << std::endl;
		_ASSERT(false);
		return nullptr;
	}

	//find the local directory of the model by using only the part of the path upto the last '/'
	std::string modelDirectory = modelPath.substr(0, modelPath.find_last_of('/')+1);

	return PullAssimpMeshFromNode(scene->mRootNode, scene, modelDirectory);
}

std::shared_ptr<Model> MeshManager::PullAssimpMeshFromNode(aiNode* node, const aiScene* scene, const std::string& localPath, std::shared_ptr<ModelNode> modelNode)
{
	//if a root node for OUR model node dosent exist
	if (modelNode == nullptr)
	{
		//get the transform for the current ASSIMP node
		aiMatrix4x4& transform = node->mTransformation;

		//convert the assimp mat4 to glm mat 4
		glm::mat4 glmMat4
		{
			static_cast<float>(transform.a1),static_cast<float>(transform.a2),static_cast<float>(transform.a3),static_cast<float>(transform.a4),
			static_cast<float>(transform.b1),static_cast<float>(transform.b2),static_cast<float>(transform.b3),static_cast<float>(transform.b4),
			static_cast<float>(transform.c1),static_cast<float>(transform.c2),static_cast<float>(transform.c3),static_cast<float>(transform.c4),
			static_cast<float>(transform.d1),static_cast<float>(transform.d2),static_cast<float>(transform.d3),static_cast<float>(transform.d4),
		};
		//create the root node with assimp transform and the assimp root name
		modelNode = std::make_shared<ModelNode>(glmMat4, std::string(node->mName.C_Str()));
	}

	//loop mesh indicies on assimp node
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		//retrieve current mesh
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//if the mesh dosent exist in storage
		if (GetMesh(std::string(mesh->mName.C_Str())) == nullptr)
		{
			//create a new mesh using our data structuress
			std::shared_ptr<Mesh> newMesh = ConstructMeshFromAssimpMesh(mesh, scene, localPath);
			//add mesh to storage
			AddMesh(std::string(mesh->mName.C_Str()), newMesh);
		}
		//add OUR mesh to OUR model node pull from storage
		modelNode->AddMesh(GetMesh(std::string(mesh->mName.C_Str())));
	}

	//loop assimp node children
	for (int j = 0; j < node->mNumChildren; j++)
	{
		//get the transform of the current assimp node child
		aiMatrix4x4& transform = node->mChildren[j]->mTransformation;

		//convert the assimp transform to glm transform
		glm::mat4 glmMat4
		{
			static_cast<float>(transform.a1),static_cast<float>(transform.a2),static_cast<float>(transform.a3),static_cast<float>(transform.a4),
			static_cast<float>(transform.b1),static_cast<float>(transform.b2),static_cast<float>(transform.b3),static_cast<float>(transform.b4),
			static_cast<float>(transform.c1),static_cast<float>(transform.c2),static_cast<float>(transform.c3),static_cast<float>(transform.c4),
			static_cast<float>(transform.d1),static_cast<float>(transform.d2),static_cast<float>(transform.d3),static_cast<float>(transform.d4),
		};

		//create OUR child node using glm mat4 and child node name
		std::shared_ptr<ModelNode> childModelNode = std::make_shared<ModelNode>(glmMat4, std::string(node->mChildren[j]->mName.C_Str()));
		//process the assimp child node
		PullAssimpMeshFromNode(node->mChildren[j], scene, localPath, childModelNode);
		//add the child node to the current node
		modelNode->AddChild(childModelNode);
	}
	std::shared_ptr<Model> newModel = std::make_shared<Model>();
	newModel->m_rootModelNode = modelNode;

	return newModel;
}

#define TRIANGLE_PRIMITIVE_SIZE 3//how
std::shared_ptr<Mesh> MeshManager::ConstructMeshFromAssimpMesh(aiMesh* assimpMesh, const aiScene* scene, const std::string& localPath)
{
	//define a vector of verticies and resize
	std::vector<Vertex> verticies;
	verticies.resize(assimpMesh->mNumVertices);

	//define a vector of elements and resize
	std::vector<GLuint> elements;
	elements.resize(TRIANGLE_PRIMITIVE_SIZE * assimpMesh->mNumFaces);


	//convert the vertex data from assimp structure to my structure
	for (size_t i = 0; i < assimpMesh->mNumVertices; i++)
	{
		Vertex& vertex = verticies[i];

		const aiVector3D& assimpVertexPosition = assimpMesh->mVertices[i];
		vertex.position.x = assimpVertexPosition.x;
		vertex.position.y = assimpVertexPosition.y;
		vertex.position.z = assimpVertexPosition.z;
		
		const aiVector3D& assimpVertexNormal = assimpMesh->mNormals[i];
		vertex.normal.x = assimpVertexNormal.x;
		vertex.normal.y = assimpVertexNormal.y;
		vertex.normal.z = assimpVertexNormal.z;

		if (assimpMesh->HasTextureCoords(0))
		{
			vertex.textureCord.x = assimpMesh->mTextureCoords[0][i].x;
			vertex.textureCord.y = assimpMesh->mTextureCoords[0][i].y;
		}
	}

	if (assimpMesh->HasFaces())
	{
		//define index to change in elements
		size_t elementIndex = 0;
		//loop facxes
		for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
		{
			//get current face
			const aiFace& face = assimpMesh->mFaces[i];
			//loop face indicies
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				//add face index to elements
				elements[elementIndex++] = face.mIndices[j];
			}
		}
	}

	//define all material maps 
	std::shared_ptr<Texture> diffuseMap = nullptr;
	std::shared_ptr<Texture> specularMap = nullptr;
	std::shared_ptr<Texture> emissionMap = nullptr;

	//if the scene has materials for meshes
	if (scene->HasMaterials())
	{
		//if assimp mesh has a material index
		if (assimpMesh->mMaterialIndex >= 0)
		{
			//get the assimp mesh material from scene
			aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];

			//load diffuse map
			diffuseMap = LoadMaterialFromAssimpMesh(material, aiTextureType_DIFFUSE, localPath);

			//load specular map
			specularMap = LoadMaterialFromAssimpMesh(material, aiTextureType_SPECULAR, localPath);

			//emission map
			emissionMap = LoadMaterialFromAssimpMesh(material, aiTextureType_EMISSIVE, localPath);
		}
	}

	//finally create a mesh
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	//load vertex data into mesh
	mesh->GenerateMesh(verticies, elements, diffuseMap, specularMap, emissionMap, 0);
	mesh->SetMeshName(std::string(assimpMesh->mName.C_Str()));
	//return the created mesh
	return mesh;
}

std::shared_ptr<Texture> MeshManager::LoadMaterialFromAssimpMesh(aiMaterial* material, aiTextureType type, const std::string& localPath)
{
	//for now only using 1 map per map type: 1 spec, 1 diffuse, 1 emission...

	//if the material has a tetxure for the passed material type
	if (material->GetTextureCount(type) > 0)
	{
		//define file location of texture
		aiString fileLocation;
		//get file location of texture
		material->GetTexture(type, 0, &fileLocation);

		//if the texture manager dosent have a texture stored with the current file name
		if (TextureManager.GetTexture(localPath + std::string(fileLocation.C_Str())) == nullptr)
		{
			//create a new texture
			std::shared_ptr<Texture> texture = std::make_shared<Texture>();
			//add the material map/texture to the created texture by using file loc
			texture->InitializeTexture(localPath + std::string(fileLocation.C_Str()));
			//add the texture to storage
			TextureManager.AddTexture(localPath + std::string(fileLocation.C_Str()), texture);
		}
		return TextureManager.GetTexture(localPath + std::string(fileLocation.C_Str()));

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
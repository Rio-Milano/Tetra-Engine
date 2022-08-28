#ifndef MESHMANAGER_H
#define MESHMANAGER_H


#include <assimp/scene.h>


#include<string>
#include<memory>
#include<map>
#include"Mesh.h"
#include"Model.h"

class MeshManager
{
public:

	static MeshManager& GetInstance();

	void AddMesh(const std::string& meshName, const std::shared_ptr<Mesh>& mesh);

	const std::shared_ptr<Mesh>& GetMesh(const std::string& meshName);

	std::shared_ptr<ModelNode> LoadModel(const std::string& modelPath);

private:
	MeshManager() = default;
	~MeshManager() = default;

	//search Assimp hirearchy for meshes
	std::shared_ptr<ModelNode> PullAssimpMeshFromNode(aiNode* node, const aiScene* scene, std::shared_ptr<ModelNode> modelNode = nullptr);
	//convert assimp mesh into opengl mesh
	std::shared_ptr<Mesh> ConstructMeshFromAssimpMesh(aiMesh* assimpMesh, const aiScene* scene);
	//extract the texture maps from the mesh
	std::shared_ptr<Texture> LoadMaterialFromAssimpMesh(aiMaterial* material, aiTextureType type);

	std::map<std::string, std::shared_ptr<Mesh>> m_meshName_mesh_map;
};

#endif
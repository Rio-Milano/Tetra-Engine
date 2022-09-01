#ifndef MESHMANAGER_H
#define MESHMANAGER_H


#include <assimp/scene.h>
#include<string>
#include<memory>
#include<unordered_map>

//class prototypes
class Mesh;
class Model;
class ModelNode;
class Texture;

class MeshManager
{
public:
	//the mesh manager is a singleton
	static MeshManager& GetInstance();

	//create a mesh outside of the class then add it
	void AddMesh(const std::string& meshName, const std::shared_ptr<Mesh>& mesh);

	//retrieve a mesh from the class
	const std::shared_ptr<Mesh>& GetMesh(const std::string& meshName);

	//uses assimp to load a model from disk then converts it into our own data structures and returns the root node for it
	std::shared_ptr<Model> LoadModel(const std::string& modelPath);

private:
	//part of singleton
	MeshManager() = default;
	~MeshManager() = default;	

	//process an assimp node by pulling meshes and processing mesh children, meanwhile making a mock of the hirearchy through modelNode
	std::shared_ptr<Model> PullAssimpMeshFromNode(aiNode* node, const aiScene* scene, const std::string& localPath, std::shared_ptr<ModelNode> modelNode = nullptr);
	
	//take assimps representation of a models mesh and converts it into our own representation
	std::shared_ptr<Mesh> ConstructMeshFromAssimpMesh(aiMesh* assimpMesh, const aiScene* scene, const std::string& localPath);
	
	//extract the texture maps from assimp mesh  nand puts it into our own data structures
	std::shared_ptr<Texture> LoadMaterialFromAssimpMesh(aiMaterial* material, aiTextureType type, const std::string& localPath);

	//hash map for meshes
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshName_mesh_map;
};

#endif
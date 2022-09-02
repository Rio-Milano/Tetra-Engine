#ifndef MESHMANAGER_H
#define MESHMANAGER_H


#include <assimp/scene.h>
#include<string>
#include<memory>
#include<unordered_map>

//class prototypes
class Mesh;


class MeshManager
{
public:
	//the mesh manager is a singleton
	static MeshManager& GetInstance();

	//create a mesh outside of the class then add it
	void AddMesh(const std::string& meshName, const std::shared_ptr<Mesh>& mesh);

	//retrieve a mesh from the class
	const std::shared_ptr<Mesh>& GetMesh(const std::string& meshName);


private:
	//part of singleton
	MeshManager() = default;
	~MeshManager() = default;	

	
	//hash map for meshes
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshName_mesh_map;
};

#endif
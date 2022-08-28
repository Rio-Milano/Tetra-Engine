#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include<string>
#include<memory>
#include<iostream>
#include<map>
#include"Mesh.h"

class MeshManager
{
public:

	static MeshManager& GetInstance();

	void AddMesh(const std::string& meshName, const std::shared_ptr<Mesh>& mesh);

	const std::shared_ptr<Mesh>& GetMesh(const std::string& meshName);

	void LoadModel(const std::string& modelPath);

private:
	MeshManager() = default;
	~MeshManager() = default;

	std::map<std::string, std::shared_ptr<Mesh>> m_meshName_mesh_map;
};

#endif
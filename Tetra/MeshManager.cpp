#include"MeshManager.h"
#include<crtdbg.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

void MeshManager::LoadModel(const std::string& modelPath)
{
	const char* path = modelPath.c_str();

}

void MeshManager::AddMesh(const std::string& meshName, const std::shared_ptr<Mesh>& mesh)
{
	if (m_meshName_mesh_map.find(meshName) == m_meshName_mesh_map.end())
		m_meshName_mesh_map[meshName] = mesh;
	else
		_ASSERT(false);//mesh already exists
}
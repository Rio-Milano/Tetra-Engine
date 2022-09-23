#include<crtdbg.h>
#include<iostream>
#include"../H/MeshManager.h"
#include"../../Structure/H/Mesh.h"

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
		//if not return a referance to static
		return fallBack;
	}
	else
	{
		//data exists so return it
		return m_meshName_mesh_map[meshName];
	}
}


void MeshManager::AddMesh(const std::string& meshName, const std::shared_ptr<Mesh>& mesh)
{
	//search store for mesh with meshName
	if (m_meshName_mesh_map.find(meshName) == m_meshName_mesh_map.end())
		//if no mesh with that name was found then add meosh
		m_meshName_mesh_map[meshName] = mesh;
	else
		//mesh already exists
		_ASSERT(false);
}
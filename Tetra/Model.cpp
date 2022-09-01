#include "Model.h"
#include"MeshManager.h"
#include<iostream>
#define MeshManager MeshManager::GetInstance()
/*
ModelNode
*/

ModelNode::ModelNode(const glm::mat4& transform)
	:
	m_assimpTransform(transform),
	m_modelNodeName("basic"), 
	m_transform(1.0f),
	m_meshes{},
	m_children{}
{
}

ModelNode::ModelNode(const glm::mat4& transform, const std::string& modelNodeName)
	:
	m_assimpTransform(transform),
	m_modelNodeName(modelNodeName),
	m_transform(1.0f),
	m_meshes{},
	m_children{}
{

}

void ModelNode::Render(Renderer& renderer, const glm::mat4& worldPrevious)
{
	//apply the user set transform then the assimp transform then the previous transform which goes back up the hirearchy
	glm::mat4 worldFinal = worldPrevious * m_assimpTransform * m_transform;

	//loop the nodes meshes
	for (const std::shared_ptr<Mesh>& mesh : m_meshes)
		//render each mesh with the current world transform
		renderer.RenderMesh(*mesh.get(), worldFinal);

	//loop the nodes children
	for (const std::shared_ptr<ModelNode>& modelNode : m_children)
		//render each child
		modelNode->Render(renderer, worldFinal);

}

void ModelNode::AddMesh(const std::shared_ptr<Mesh>& mesh)
{
	//add mesh to meshes vector
	m_meshes.emplace_back(mesh);
}

void ModelNode::AddMesh(const std::string& meshName)
{
	//pull mesh from mesh manager
	const std::shared_ptr<Mesh>& mesh = MeshManager.GetMesh(meshName);
	//add mesh
	m_meshes.emplace_back(mesh);
}

void ModelNode::AddChild(const std::shared_ptr<ModelNode>& modelNode)
{
	//add child node to children
	m_children.emplace_back(modelNode);
}

void ModelNode::AddChildren(const std::vector<std::shared_ptr<ModelNode>> modelNodes)
{
	//merge the children vector and the additional children vector by adding them on end
	m_children.insert(m_children.end(), modelNodes.begin(), modelNodes.end());
}

const std::string& ModelNode::GetNodeName()const
{
	//gets name of node
	return m_modelNodeName;
}

const glm::mat4& ModelNode::GetTransform()const
{
	return m_transform;
}

void ModelNode::SetTransform(const glm::mat4& transform)
{
	//apply user set transform
	m_transform = transform;
}

const glm::mat4& ModelNode::GetAssimpNodeTransform()const
{
	//gets transform set by assimp
	return m_assimpTransform;
}

void ModelNode::SetAssimpNodeTransform(const glm::mat4& transform)
{
	m_assimpTransform = transform;
}


const std::vector<std::shared_ptr<Mesh>>& ModelNode::GetMeshes()const
{
	//get all meshes attached to node
	return m_meshes;
}

const std::vector<std::shared_ptr<ModelNode>>& ModelNode::GetChildren()const
{
	//get all children attached to node
	return m_children;
}



/*
Model
*/

Model::Model(const std::shared_ptr<ModelNode>& rootNode)
	:m_rootModelNode(rootNode)
{
}

const std::shared_ptr<ModelNode>& Model::GetRoot()const
{
	return m_rootModelNode;
}

void Model::Render(Renderer& renderer)
{
	//trigger break point if root is null
	_ASSERT(m_rootModelNode != nullptr);
	//else render hirearchy
	m_rootModelNode->Render(renderer);
}

std::shared_ptr<ModelNode> Model::FindNode(const std::string& nodeName, std::shared_ptr<ModelNode> node)const
{
	//if current node is null
	if (node == nullptr)
		//assign the root node
		node = m_rootModelNode;

	//if current node has the name were looking for
	if (node->GetNodeName() == nodeName)
		//return current node
		return node;

	//loop current node children
	for (const std::shared_ptr<ModelNode>& childNode : node->m_children)
	{
		//call find node for current child
		std::shared_ptr<ModelNode> foundNode = FindNode(nodeName, childNode);
		//if found node returned node then a node with passed name was fond
		if (foundNode != nullptr)
			//so return node
			return foundNode;
	}
	//if node with a name was not found within the current node or its children and beyond then return nullptr
	return nullptr;
}

void Model::SetRoot(const std::shared_ptr<ModelNode>& newRoot)
{
	m_rootModelNode = newRoot;
}

std::shared_ptr<Mesh> Model::FindMesh(const std::string& meshName, std::shared_ptr<ModelNode> node)const
{
	//if current node not set
	if (node == nullptr)
		//assign current node the root node
		node = m_rootModelNode;

	//loop meshes of current node
	for (const std::shared_ptr<Mesh>& mesh : node->m_meshes)
	{
		//if current mesh has passed name
		if (mesh->GetMeshName() == meshName)
			//return current mesh
			return mesh;
	}

	//loop children of current node
	for (const std::shared_ptr<ModelNode>& childNode : node->m_children)
	{
		//recursively call find mesh for each of the current nodes children
		std::shared_ptr<Mesh> foundMesh = FindMesh(meshName, childNode);
		//if the call returned a mesh then a mesh with the passed name was found
		if (foundMesh != nullptr)
			//so return the found mesh
			return foundMesh;
	}

	//if the current node did not have a mesh with the passed name nor its children or beyold them return nullptr
	return nullptr;

}

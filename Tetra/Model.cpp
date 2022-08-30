#include "Model.h"
#include"MeshManager.h"
#include<iostream>
#define MeshManager MeshManager::GetInstance()
/*
ModelNode
*/

ModelNode::ModelNode(const glm::mat4& transform)
	:m_transform_mat4(transform), m_nodeTransform(1.0f)
{

}

ModelNode::ModelNode(const glm::mat4& transform, const std::string& modelNodeName)
	:m_transform_mat4(transform), m_modelNodeName(modelNodeName), m_nodeTransform(1.0f)
{

}

void ModelNode::Render(Renderer& renderer, const glm::mat4& worldPrevious)
{
	glm::mat4 worldFinal = worldPrevious * m_transform_mat4 * m_nodeTransform;

	for (const std::shared_ptr<Mesh>& mesh : m_meshes)
	{
		renderer.RenderMesh(*mesh.get(), worldFinal);
	}

	for (const std::shared_ptr<ModelNode>& modelNode : m_children)
	{
		modelNode->Render(renderer, worldFinal);
	}

}

void ModelNode::AddMesh(const std::shared_ptr<Mesh>& mesh)
{
	m_meshes.emplace_back(mesh);
}

void ModelNode::AddMesh(const std::string& meshName)
{
	m_meshes.emplace_back(MeshManager.GetMesh(meshName));
}

void ModelNode::AddChild(const std::shared_ptr<ModelNode>& modelNode)
{
	m_children.emplace_back(modelNode);
}

void ModelNode::AddChildren(const std::vector<std::shared_ptr<ModelNode>> modelNodes)
{
	m_children.insert(m_children.end(), modelNodes.begin(), modelNodes.end());
}

const std::string& ModelNode::GetNodeName()const
{
	return m_modelNodeName;
}


/*
Model
*/

Model::Model(const std::shared_ptr<ModelNode>& rootNode)
	:m_rootModelNode(rootNode)
{
}

void Model::Render(Renderer& renderer)
{
	_ASSERT(m_rootModelNode != nullptr);
	m_rootModelNode->Render(renderer);
}


std::shared_ptr<ModelNode> Model::FindNode(const std::string& nodeName, std::shared_ptr<ModelNode> node)const
{
	if (node == nullptr)
		node = m_rootModelNode;

	if (node->GetNodeName() == nodeName)
		return node;

	for (const std::shared_ptr<ModelNode>& childNode : node->m_children)
	{
		std::shared_ptr<ModelNode> foundNode = FindNode(nodeName, childNode);
		if (foundNode != nullptr)
			return foundNode;
	}
	return nullptr;
}

std::shared_ptr<Mesh> Model::FindMesh(const std::string& meshName, std::shared_ptr<ModelNode> node)const
{
	if (node == nullptr)
		node = m_rootModelNode;

	for (const std::shared_ptr<Mesh>& mesh : node->m_meshes)
	{
		if (mesh->GetMeshName() == meshName)
			return mesh;
	}


	for (const std::shared_ptr<ModelNode>& childNode : node->m_children)
	{
		std::shared_ptr<Mesh> foundMesh = FindMesh(meshName, childNode);
		if (foundMesh != nullptr)
			return foundMesh;
	}

	return nullptr;

}
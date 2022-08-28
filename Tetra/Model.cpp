#include "Model.h"
#include"MeshManager.h"
#include<iostream>
#define MeshManager MeshManager::GetInstance()
/*
ModelNode
*/

ModelNode::ModelNode(const glm::mat4& transform)
	:m_transform_mat4(transform)
{

}

ModelNode::ModelNode(const glm::mat4& transform, const std::string& modelNodeName)
	:m_transform_mat4(transform), m_modelNodeName(modelNodeName)
{

}

void ModelNode::Render(Renderer& renderer, const glm::mat4& worldPrevious)
{
	glm::mat4 worldFinal = m_transform_mat4 * worldPrevious;

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

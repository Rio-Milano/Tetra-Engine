#include "Model.h"
#include"MeshManager.h"
#define MeshManager MeshManager::GetInstance()
/*
ModelNode
*/

ModelNode::ModelNode(const Transform& transform)
	:m_transform(transform)
{

}

ModelNode::ModelNode(const Transform& transform, const std::string& modelNodeName)
	:m_transform(transform), m_modelNodeName(modelNodeName)
{

}

void ModelNode::Render(Renderer& renderer, const glm::mat4& worldPrevious)const
{
	glm::mat4 worldCurrent(1.0f);
	worldCurrent = glm::translate(worldCurrent, m_transform.m_position);
	
	if(glm::dot(m_transform.m_rotation, m_transform.m_rotation) != 0.0f)
		worldCurrent = glm::rotate(worldCurrent, m_transform.m_angle, m_transform.m_rotation);
	
	worldCurrent = glm::scale(worldCurrent, m_transform.m_scale);

	glm::mat4 worldFinal = worldCurrent * worldPrevious;

	std::cout << m_modelNodeName << std::endl;

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

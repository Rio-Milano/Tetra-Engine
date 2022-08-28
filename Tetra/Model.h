#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include<memory>
#include<crtdbg.h>

#include"Mesh.h"
#include"glmIncludes.h"
#include"Renderer.h"


class ModelNode
{
public:
	explicit ModelNode(const glm::mat4& transform);
	explicit ModelNode(const glm::mat4& transform, const std::string& modelNodeName);


	void Render(Renderer& renderer, const glm::mat4& worldPrevious = glm::mat4(1.0f));

	void AddMesh(const std::string& meshName);
	void AddMesh(const std::shared_ptr<Mesh>& mesh);
	void AddChild(const std::shared_ptr<ModelNode>& modelNode);

	const std::string& GetNodeName()const;

	glm::mat4 m_transform_mat4;

private:
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<std::shared_ptr<ModelNode>> m_children;
	std::string m_modelNodeName{"basic"};
};


class Model
{
public:
	explicit Model(const std::shared_ptr<ModelNode>& rootNode);
	Model() = default;
	~Model() = default;

	void Render(Renderer& renderer);

protected:
	std::shared_ptr<ModelNode> m_rootModelNode = nullptr;
};

#endif
#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include<memory>
#include<crtdbg.h>

#include"Mesh.h"
#include"glmIncludes.h"
#include"Renderer.h"

struct Transform
{
	explicit Transform(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec3 rotation = { 0.0f, 0.0f, 0.0f }, const float angle = 0.0f, const glm::vec3& scale = { 1.0f, 1.0f, 1.0f })
		:m_position(position), m_rotation(rotation), m_angle(angle), m_scale(scale) {};

	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
	float m_angle{0.0f};
	glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
};


class ModelNode
{
public:
	explicit ModelNode(const Transform& transform);
	explicit ModelNode(const Transform& transform, const std::string& modelNodeName);


	void Render(Renderer& renderer, const glm::mat4& worldPrevious = glm::mat4(1.0f))const;

	void AddMesh(const std::string& meshName);
	void AddMesh(const std::shared_ptr<Mesh>& mesh);
	void AddChild(const std::shared_ptr<ModelNode>& modelNode);

	const std::string& GetNodeName()const;

	Transform m_transform;

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
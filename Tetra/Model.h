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
	friend class Model;

public:
	//ctor/dtor
	explicit ModelNode(const glm::mat4& transform);//basic ctor
	explicit ModelNode(const glm::mat4& transform, const std::string& modelNodeName);//set name aswell
	~ModelNode() = default;

	//recursively search the hirearchy and render each node
	void Render(Renderer& renderer, const glm::mat4& worldPrevious = glm::mat4(1.0f));

	//add a mesh to the current node by using a name to pull a mesh from the mesh manager
	void AddMesh(const std::string& meshName);
	//add a mesh by directly takinng in a mesh
	void AddMesh(const std::shared_ptr<Mesh>& mesh);

	//add child model node to current node
	void AddChild(const std::shared_ptr<ModelNode>& modelNode);
	//add multiple children to current node
	void AddChildren(const std::vector<std::shared_ptr<ModelNode>> modelNodes);

	//Getters
	const std::string& GetNodeName()const;
	const glm::mat4& GetAssimpNodeTransform()const;
	const glm::mat4& GetTransform()const;
	const std::vector<std::shared_ptr<Mesh>>& GetMeshes()const;
	const std::vector<std::shared_ptr<ModelNode>>& GetChildren()const;
	
	//Setters
	void SetTransform(const glm::mat4& transform);
	void SetAssimpNodeTransform(const glm::mat4& transform);
private:
	glm::mat4 m_assimpTransform;
	glm::mat4 m_transform;


	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::vector<std::shared_ptr<ModelNode>> m_children;
	std::string m_modelNodeName;
};


class Model
{
public:
	//ctors and dtors
	explicit Model(const std::shared_ptr<ModelNode>& rootNode);
	Model() = default;
	~Model() = default;

	//render the model by starting at root of hirearchy
	void Render(Renderer& renderer);
	
	//utility functions for finding nodes and meshes within the hirearchy
	std::shared_ptr<ModelNode> FindNode(const std::string& nodeName, std::shared_ptr<ModelNode> node = nullptr)const;
	std::shared_ptr<Mesh> FindMesh(const std::string& meshName, std::shared_ptr<ModelNode> node = nullptr)const;

	//Getters
	const std::shared_ptr<ModelNode>& GetRoot()const;

	//Setters
	void SetRoot(const std::shared_ptr<ModelNode>& newRoot);

private:
	std::shared_ptr<ModelNode> m_rootModelNode = nullptr;
};

#endif
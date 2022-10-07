#include "../H/Model.h"

#include"../../Managers/H/MeshManager.h"
#define MeshManager MeshManager::GetInstance()

#include"../../Managers/H/TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include"../../Managers/H/ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include<iostream>

/*
ModelNode
*/

bool ModelNode::DrawNormals = false;

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

const bool& ModelNode::GetDrawOutline()const
{
	return m_drawOutline;
}


void ModelNode::Render(Renderer& renderer, Shader& shader, const glm::mat4& worldPrevious)
{
	//apply the user set transform then the assimp transform then the previous transform which goes back up the hirearchy
	glm::mat4 worldFinal = worldPrevious * m_assimpTransform * m_transform;

	//loop the nodes meshes
	for (/*const*/ std::shared_ptr<Mesh>& mesh : m_meshes)
	{
		if (this->m_drawOutline)
			DrawOutline(*mesh.get(), renderer, shader, worldFinal);
		else
			renderer.RenderMesh(*mesh.get(), worldFinal, shader);//render each mesh with the current world transform

		if(DrawNormals)
			renderer.RenderMesh(*mesh.get(), worldFinal, ShaderManager.GetShader("Draw-Normals"));//render each mesh with the current world transform

	}

	//loop the nodes children
	for (const std::shared_ptr<ModelNode>& modelNode : m_children)
		//render each child
		modelNode->Render(renderer, shader, worldFinal);

}

#define DEFAULT_OUTLINE_COLOR glm::vec3(1.0f)
#define DEFAULT_OUTLINE_SCALE glm::vec3(1.04f)
void ModelNode::DrawOutline(Mesh& mesh, Renderer& renderer, Shader& shader, const glm::mat4& transform)
{
	glEnable(GL_STENCIL_TEST);//enable the stencil buffer


	glStencilMask(0xFF);//enable writing

	glStencilFunc(GL_ALWAYS, 1, 0xFF);//allways pass fragment no matter value
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//replace every stencil value with referance value => 1
	renderer.RenderMesh(mesh, transform, shader);//render the base mesh

	glm::mat4 newTransform = glm::scale(transform, DEFAULT_OUTLINE_SCALE);//define scaled the mesh transform
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//for each fragment if the stencil value is not equal to 1 then pass the test (now only fragment edges of scaled mesh will pass stencil test)
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//for every passed test set stencil value to referance => 1
	
	Shader& new_shader = ShaderManager.GetShader("lightCubeShader");//get shader for outline
	new_shader.SetUniform3fv(new_shader.GetLocation("cubeColor"), DEFAULT_OUTLINE_COLOR);//set uniform color in shader
	
	renderer.RenderMesh(mesh, newTransform, new_shader);//render the scaled mesh with new transform and new rules applied to the stencil buffer


	glClear(GL_STENCIL_BUFFER_BIT);//clear stencil buffer
	glDisable(GL_STENCIL_TEST);//disable stencil buffer as no longer needed
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


std::vector<std::shared_ptr<Mesh>>& ModelNode::GetMeshes()
{
	//get all meshes attached to node
	return m_meshes;
}

std::vector<std::shared_ptr<ModelNode>>& ModelNode::GetChildren()
{
	//get all children attached to node
	return m_children;
}

void ModelNode::SetDrawOutline(const bool& flag)
{
	m_drawOutline = flag;
}




/*
Model
*/

Model::Model(const std::shared_ptr<ModelNode>& rootNode)
	:m_rootModelNode(rootNode)
{
}


void Model::CreateInstances(std::vector<glm::mat4>* transforms)
{
	//Check that transforms is not nullptr
	_ASSERT(transforms);

	//Check that there will be more than 1 instance drawn
	_ASSERT(transforms->size() > 1);


	GLuint instanceArrayBuffer = 0;
	glGenBuffers(1, &instanceArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, transforms->size() * sizeof(glm::mat4), transforms->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	ProcessInstances_Sub_Meshes(instanceArrayBuffer, m_rootModelNode);

}

void Model::ProcessInstances_Sub_Meshes(const GLuint& instanceArray, const std::shared_ptr<ModelNode>& node)
{
	static constexpr size_t
		mat4Size = sizeof(glm::mat4),
		vec4Size = sizeof(glm::vec4);

	glBindBuffer(GL_ARRAY_BUFFER, instanceArray);
	GLint bufferSize = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	GLsizei instances = static_cast<GLsizei>(static_cast<float>(bufferSize) / static_cast<float>(mat4Size));
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//Loop through meshes of node
	for (const std::shared_ptr<Mesh>& meshPtr : node->GetMeshes())
	{
		glBindVertexArray(meshPtr->m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, instanceArray);

		meshPtr->m_numberOfInstances = instances;

		unsigned long long MemoryOffset = 0;
		GLuint AttributeIndex = SHADER_LAYOUT_INDEX_OFFSET;

		glEnableVertexAttribArray(AttributeIndex);
		glVertexAttribPointer(AttributeIndex, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)MemoryOffset);
		glVertexAttribDivisor(AttributeIndex, 1);

		MemoryOffset += vec4Size;
		AttributeIndex++;
		glEnableVertexAttribArray(AttributeIndex);
		glVertexAttribPointer(AttributeIndex, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)MemoryOffset);
		glVertexAttribDivisor(AttributeIndex, 1);

		MemoryOffset += vec4Size;
		AttributeIndex++;
		glEnableVertexAttribArray(AttributeIndex);
		glVertexAttribPointer(AttributeIndex, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)MemoryOffset);
		glVertexAttribDivisor(AttributeIndex, 1);

		MemoryOffset += vec4Size;
		AttributeIndex++;
		glEnableVertexAttribArray(AttributeIndex);
		glVertexAttribPointer(AttributeIndex, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)MemoryOffset);
		glVertexAttribDivisor(AttributeIndex, 1);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	for (const std::shared_ptr<ModelNode>& child : node->m_children)
	{
		ProcessInstances_Sub_Meshes(instanceArray, child);
	}
}

const std::shared_ptr<ModelNode>& Model::GetRoot()const
{
	return m_rootModelNode;
}

void Model::Render(Renderer& renderer, Shader& shader)
{
	//trigger break point if root is null
	_ASSERT(m_rootModelNode != nullptr);
	//else render hirearchy
	m_rootModelNode->Render(renderer, shader);
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

Model::Model(const std::string& modelPath)
{
	LoadModel(modelPath);
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



//recieve a model location
void Model::LoadModel(const std::string& modelPath)
{
	//get the path as a const char *
	const char* path = modelPath.c_str();

	//define an importer object to load the model
	Assimp::Importer importer;

	//tell importer to read path and force triangulation then hold the returned scene ptr
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	//if scene is empty or scene failed some point or root node of scene is empty then report error
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "Failed to load model, path :" << modelPath << std::endl;
		std::cout << "Error (Assimp) :" << importer.GetErrorString() << std::endl;
		_ASSERT(false);
		return;
	}

	//find the local directory of the model by using only the part of the path upto the last '/' and including '/'
	std::string modelDirectory = modelPath.substr(0, modelPath.find_last_of('/') + 1);

	//traverse the assimp hirearchy and pull each node into a node of our own recursively hich returns a model which we can return
	m_rootModelNode = PullAssimpMeshFromNode(scene->mRootNode, scene, modelDirectory);
}


std::shared_ptr<ModelNode> Model::PullAssimpMeshFromNode(aiNode* node, const aiScene* scene, const std::string& localPath, std::shared_ptr<ModelNode> modelNode)
{
	//if a root node for current model  dosent exist
	if (modelNode == nullptr)
	{
		//get the transform for the current ASSIMP node
		aiMatrix4x4& transform = node->mTransformation;

		//convert the assimp mat4 to glm mat 4
		glm::mat4 glmMat4
		{
			static_cast<float>(transform.a1),static_cast<float>(transform.a2),static_cast<float>(transform.a3),static_cast<float>(transform.a4),
			static_cast<float>(transform.b1),static_cast<float>(transform.b2),static_cast<float>(transform.b3),static_cast<float>(transform.b4),
			static_cast<float>(transform.c1),static_cast<float>(transform.c2),static_cast<float>(transform.c3),static_cast<float>(transform.c4),
			static_cast<float>(transform.d1),static_cast<float>(transform.d2),static_cast<float>(transform.d3),static_cast<float>(transform.d4),
		};
		//create the root node with converted assimp transform and the assimp root name
		modelNode = std::make_shared<ModelNode>(glmMat4, std::string(node->mName.C_Str()));
	}

	//loop meshes on current assimp node
	for (int i = 0; i < static_cast<int>(node->mNumMeshes); i++)
	{
		//retrieve current mesh
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//if the mesh dosent exist in storage
		if (MeshManager.GetMesh(std::string(mesh->mName.C_Str())) == nullptr)
		{
			//create a new mesh using our data structuress
			std::shared_ptr<Mesh> newMesh = ConstructMeshFromAssimpMesh(mesh, scene, localPath);
			//add mesh to storage
			MeshManager.AddMesh(std::string(mesh->mName.C_Str()), newMesh);
		}
		//add OUR mesh to OUR model node pull from storage
		modelNode->AddMesh(MeshManager.GetMesh(std::string(mesh->mName.C_Str())));
	}

	//loop assimp node children
	for (int j = 0; j < static_cast<int>(node->mNumChildren); j++)
	{
		//get the transform of the current assimp node child
		aiMatrix4x4& transform = node->mChildren[j]->mTransformation;

		//convert the assimp transform to glm transform
		glm::mat4 glmMat4
		{
			static_cast<float>(transform.a1),static_cast<float>(transform.a2),static_cast<float>(transform.a3),static_cast<float>(transform.a4),
			static_cast<float>(transform.b1),static_cast<float>(transform.b2),static_cast<float>(transform.b3),static_cast<float>(transform.b4),
			static_cast<float>(transform.c1),static_cast<float>(transform.c2),static_cast<float>(transform.c3),static_cast<float>(transform.c4),
			static_cast<float>(transform.d1),static_cast<float>(transform.d2),static_cast<float>(transform.d3),static_cast<float>(transform.d4),
		};

		//create OUR child node using glm mat4 and child node name
		std::shared_ptr<ModelNode> childModelNode = std::make_shared<ModelNode>(glmMat4, std::string(node->mChildren[j]->mName.C_Str()));
		//process the assimp child node but with our new child node
		PullAssimpMeshFromNode(node->mChildren[j], scene, localPath, childModelNode);
		//add the child node to the current node
		modelNode->AddChild(childModelNode);
	}
	
	//return the created model
	return modelNode;
}

#define TRIANGLE_PRIMITIVE_SIZE 3//number of vertices per face
std::shared_ptr<Mesh> Model::ConstructMeshFromAssimpMesh(aiMesh* assimpMesh, const aiScene* scene, const std::string& localPath)
{
	//create vectors on heap for each vertex attribute
	std::vector<glm::vec3>* positionsPtr = new std::vector<glm::vec3>;
	std::vector<glm::vec3>* normalsPtr = new std::vector<glm::vec3>;
	std::vector<glm::vec2>* texCoordsPtr = new std::vector<glm::vec2>;

	//resize each vector to match assimp vertex count
	positionsPtr->resize(assimpMesh->mNumVertices);
	texCoordsPtr->resize(assimpMesh->mNumVertices);
	normalsPtr->resize(assimpMesh->mNumVertices);

	//define a vector of elements and resize
	std::vector<GLuint>* elementsPtr = new std::vector<unsigned int>;
	elementsPtr->resize(TRIANGLE_PRIMITIVE_SIZE * assimpMesh->mNumFaces);


	//convert the vertex data from assimp structure to my structure
	for (size_t i = 0; i < assimpMesh->mNumVertices; i++)
	{
		//grab a referance of current position
		glm::vec3& position = (*positionsPtr)[i];
		//provess positions
		const aiVector3D& assimpVertexPosition = assimpMesh->mVertices[i];
		position.x = assimpVertexPosition.x;
		position.y = assimpVertexPosition.y;
		position.z = assimpVertexPosition.z;

		//get current normal
		glm::vec3& normal = (*normalsPtr)[i];
		//process normals
		const aiVector3D& assimpVertexNormal = assimpMesh->mNormals[i];
		normal.x = assimpVertexNormal.x;
		normal.y = assimpVertexNormal.y;
		normal.z = assimpVertexNormal.z;

		//provess texture cords only if has them
		if (assimpMesh->HasTextureCoords(0))
		{
			//get current texcoord
			glm::vec2& textureCord = (*texCoordsPtr)[i];
			//process texture cords
			textureCord.x = assimpMesh->mTextureCoords[0][i].x;
			textureCord.y = assimpMesh->mTextureCoords[0][i].y;
		}
	}

	//process indexed vertices
	if (assimpMesh->HasFaces())
	{
		//define index to change element index
		size_t elementIndex = 0;
		//loop faces
		for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
		{
			//get current face
			const aiFace& face = assimpMesh->mFaces[i];
			//loop face indicies
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				//add face index to elements
				(*elementsPtr)[elementIndex++] = face.mIndices[j];
			}
		}
	}

	//finally create a mesh
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	
	//load vertex data into mesh
	mesh->GenerateMesh(positionsPtr, normalsPtr, texCoordsPtr, {}, elementsPtr, 0);
	
	//set flag to tell mesh to delete vertex attributes on heap when deconstructor called
	//set mesh name using assimp mesh name
	mesh->SetMeshName(std::string(assimpMesh->mName.C_Str()));

	//if the scene has materials for meshes
	if (scene->HasMaterials())
	{
		//if assimp mesh has a material index
		if (assimpMesh->mMaterialIndex >= 0)
		{
			//get the assimp mesh material from scene
			aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];

			//load diffuse map
			mesh->GetMaterial()->m_diffuse = LoadMaterialFromAssimpMesh(material, aiTextureType_DIFFUSE, localPath);

			//load specular map
			mesh->GetMaterial()->m_specular = LoadMaterialFromAssimpMesh(material, aiTextureType_SPECULAR, localPath);

			//emission map
			mesh->GetMaterial()->m_emission = LoadMaterialFromAssimpMesh(material, aiTextureType_EMISSIVE, localPath);
		}
	}
	//return the created mesh
	return mesh;
}

std::shared_ptr<Texture> Model::LoadMaterialFromAssimpMesh(aiMaterial* material, aiTextureType type, const std::string& localPath)
{
	//for now only using 1 map per map type: 1 spec, 1 diffuse, 1 emission...

	//if the material has a tetxure for the passed material type
	if (material->GetTextureCount(type) > 0)
	{
		//define file location of texture
		aiString fileLocation;
		//get file location of texture
		material->GetTexture(type, 0, &fileLocation);

		//if the texture manager dosent have a texture stored with the current file name
		if (TextureManager.GetTexture(localPath + std::string(fileLocation.C_Str())) == nullptr)
		{
			//create a new texture
			std::shared_ptr<Texture> texture = std::make_shared<Texture>();
			
			bool ls = false;
			if (type == aiTextureType_DIFFUSE) ls = true;

			//add the material map/texture to the created texture by using file loc
			texture->InitializeTexture(localPath + std::string(fileLocation.C_Str()), false, ls);
			//add the texture to storage
			TextureManager.AddTexture(localPath + std::string(fileLocation.C_Str()), texture);
		}
		return TextureManager.GetTexture(localPath + std::string(fileLocation.C_Str()));

	}
	//return null as material has no texture for the passed type
	return nullptr;
}
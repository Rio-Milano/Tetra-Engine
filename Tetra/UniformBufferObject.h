#ifndef UNIFORMBUFFEROBJECT_H
#define UNIFORMBUFFEROBJECT_H

#include<glad/glad.h>
#include<string>
#include<unordered_map>

class UniformBufferObject
{
	friend class ShaderManager;

public:
	struct Element
	{
		GLsizei
			m_elementSize,//this is the size deffined by glsl (base offset)
			m_elementOffset;//this offset as deffined by opengl specification (aligned offset)
	};

	UniformBufferObject() = default;
	UniformBufferObject(const GLsizei& bufferSize, const GLuint& bindingPoint, const std::string& uniformBlockName);
	~UniformBufferObject();

	//this uses stored element data to update data in the ubo
	void SetBufferElement(const std::string& elementName, void* data)const;
	
	//this takes and stores size and offset data about an element that the ubo will hold
	void SetElementData(const std::string& elementName, const Element& element);

private:
	GLuint 
		m_ubo,//buffer object id
		m_bindingPoint;//index to bind to

	std::string m_uniformBlockName;//the name of the uniform block in the shader
	
	std::unordered_map<std::string, Element> m_elementDescription;
};

#endif
#include "UniformBufferObject.h"
#include"glmIncludes.h"
#include<iostream>
UniformBufferObject::UniformBufferObject(const GLsizei& bufferSize, const GLuint& bindingPoint, const std::string& uniformBlockName)
	:
		m_ubo(NULL),
		m_bindingPoint(bindingPoint),
		m_uniformBlockName(uniformBlockName),
		m_elementDescription(NULL)
{
	//create the uniform buffer
	glGenBuffers(1, &m_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//set the binding point of the uniform buffer object
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_ubo, 0, bufferSize);
}

UniformBufferObject::~UniformBufferObject()
{
	glDeleteBuffers(1, &m_ubo);
}

void UniformBufferObject::SetElementData(const std::string& elementName, const UniformBufferObject::Element& element)
{
	//overwriting is ok
	m_elementDescription[elementName] = element;
}

void UniformBufferObject::SetBufferElement(const std::string& elementName, void* data)const
{
	//if element name dosent exist in store then break point
	_ASSERT(m_elementDescription.find(elementName) != m_elementDescription.end());

	//get element
	const UniformBufferObject::Element& element = m_elementDescription.find(elementName)->second;

	//get element attributes
	const GLsizei& size = element.m_elementSize;
	const GLsizei& offset = element.m_elementOffset;

	//add data into ubo according to stored element data
	glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


}
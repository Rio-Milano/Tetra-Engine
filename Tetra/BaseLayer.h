#ifndef BASELAYER_H
#define BASELAYER_H

#include"Window.h"
#include"Camera.h"
#include"Renderer.h"
#include"glmIncludes.h"
#include"Shader.h"
#include"Entity.h"

#include<string>
#include<vector>
#include<memory>

class BaseLayer
{
	friend class Application;

protected:
	virtual void Start() {};
	virtual void Update(float dt) {};
	virtual void Render() {};
	virtual void End() {};

	Renderer m_renderer;
	Camera m_camera;

private:
	void CreateLayer(const glm::vec<2, int> windowSize, const std::string& windowName);
	void DestroyLayer();
	void BaseUpdate(const float& dt);

	void InitGLAD();
	void CreateShader();

};

#endif
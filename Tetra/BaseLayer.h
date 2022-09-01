#ifndef BASELAYER_H
#define BASELAYER_H

#include"Window.h"
#include"Camera.h"
#include"Renderer.h"
#include"glmIncludes.h"
#include"Shader.h"
#include"Entity.h"
#include"LightManager.h"

#include<string>
#include<vector>
#include<memory>

class BaseLayer
{
	friend class Application;

protected:
	virtual void Start() {};
	virtual void Update(float dt) {};
	virtual void ImGUI() {};
	virtual void Render() {};
	virtual void End() {};

	Renderer m_renderer;
	Camera m_camera;
	LightManager m_lightManager;

private:
	void CreateLayer(const glm::vec<2, int> windowSize, const std::string& windowName);
	void InitializeImGui();
	void DestroyLayer();
	void BaseRender();
	void BaseUpdate(const float& dt);
	void BaseimGUI();
	void InitGLAD();
	void CreateShader();

};

#endif
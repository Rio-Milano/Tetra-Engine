#ifndef BASELAYER_H
#define BASELAYER_H

#include"../../Graphics/H/Window.h"
#include"../../Structure/H/Camera.h"
#include"../../Graphics/H/Renderer.h"
#include"../../../Requirements/glmIncludes.h"
#include"../../../OpenGL_Abstraction/H/Shader.h"
#include"../../../Entities/H/Entity.h"
#include"../../Managers/H/LightManager.h"
#include"../../../Entities/H/BaseGrid.h"

#include<string>
#include<vector>
#include<memory>



class BaseLayer
{
	friend class Application;
	friend class LightManager;

protected:
	virtual void Start() {};
	virtual void Update(float dt) {};
	virtual void ImGUI() {};
	
	virtual void PreRender(Shader* overrideShader) {};
	virtual void Render(Shader* overideShader) {};
	virtual void PostRender() {};
	virtual void End() {};

	Renderer m_renderer;
	Camera m_camera;
	LightManager m_lightManager;

private:
	//internal helpers
	void CreateLayer(const glm::vec<2, int> windowSize, const std::string& windowName);
	void CreateUniformBuffers();
	void InitializeImGui();
	void DestroyLayer();
	void BaseRender();
	void BaseUpdate(const float& dt);
	void BaseimGUI();
	void InitGLAD();
	void CreateShader();
	void UpdateUniformBuffers();

	std::shared_ptr<BaseGrid> m_grid{nullptr};

};

#endif
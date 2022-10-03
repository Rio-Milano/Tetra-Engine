#ifndef SANDBOXLAYER_H
#define SANDBOXLAYER_H

#include"BaseLayer.h"
class PostProcessing;

class SandBoxLayer : public BaseLayer
{

public:
	//ctors-dtors
	SandBoxLayer() = default;
	~SandBoxLayer() = default;

	//BaseLayer virtual functions
	void Start() override final;
	void Update(float dt) override final;
	void PreRender(Shader* overrideShader) override final;
	void Render(Shader* overideShader) override final;
	void PostRender() override final;
	void End() override final;
	void ImGUI() override final;

private:
	//Imgui flags used in layer update and rendering
	bool m_wireframeMode{false};
	bool m_pauseSimulation{false};
	bool m_drawLightingAsMeshes{true};

	float gamma = 2.2f;
	//entity list
	std::vector<std::shared_ptr<Entity>> m_entities;

	std::shared_ptr<PostProcessing> m_postProcessing;
};

#endif
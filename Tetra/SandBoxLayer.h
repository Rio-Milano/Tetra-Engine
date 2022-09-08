#ifndef SANDBOXLAYER_H
#define SANDBOXLAYER_H

#include"BaseLayer.h"
class Framebuffer;

class SandBoxLayer : public BaseLayer
{
public:
	//ctors-dtors
	SandBoxLayer() = default;
	~SandBoxLayer() = default;

	//BaseLayer virtual functions
	void Start() override final;
	void Update(float dt) override final;
	void Render() override final;
	void End() override final;
	void ImGUI() override final;

private:
	//Imgui flags used in layer update and rendering
	bool m_wireframeMode{false};
	bool m_pauseSimulation{false};
	float m_XsampleOffsetMagnitude{ 1.0f / 1920.0f };
	float m_YsampleOffsetMagnitude{ 1.0f / 1080.0f };
	bool m_enableSharpeningKernel{ false };
	bool m_enableWeirdKernel{ false };
	bool m_enableBlurKernel{ false };
	bool m_edgeDetectionKernel{ false };
	bool m_enableColorBufferInversion{ false };
	bool m_enableGreyScaleColorBuffer{ false };

	//entity list
	std::vector<std::shared_ptr<Entity>> m_entities;

	std::shared_ptr<Framebuffer> m_frameBuffer;
};

#endif
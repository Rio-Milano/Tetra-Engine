#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include<memory>

#include"Framebuffer.h"

class Mesh;
class Renderer;

class PostProcessing
{

public:
	struct Config
	{
		float m_XsampleOffsetMagnitude{ 1.0f / 1920.0f };
		float m_YsampleOffsetMagnitude{ 1.0f / 1080.0f };
		bool m_enableSharpeningKernel{ false };
		bool m_enableWeirdKernel{ false };
		bool m_enableBlurKernel{ false };
		bool m_edgeDetectionKernel{ false };
		bool m_enableColorBufferInversion{ false };
		bool m_enableGreyScaleColorBuffer{ false };
	};

	PostProcessing(const glm::vec2& viewPortSize);

	//this switches the active frame buffer to the one we create
	void Render_To_Off_Screen_Buffer();


	//render the quad
	void Render_FrameBuffer(Renderer& renderer);

	Config m_config;

private:
	Framebuffer m_frameBuffer;
	std::shared_ptr<Mesh> m_quad;
};

#endif
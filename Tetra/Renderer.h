
#ifndef RENDERER_H
#define RENDERER_H

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include<string>

#include"Mesh.h"
#include"Window.h"
#include"Shader.h"
#include"Camera.h"

class Mesh;

class Renderer
{
public:

	Renderer() = default;
	~Renderer() = default;

	void CreateWindow(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName);
	void InitRenderer();
	static void UpdateViewportWhenWindowSizeChanged(GLFWwindow* window, int windowWidth, int windowHeight);

	void RenderMesh(const Mesh& mesh, const glm::mat4& worldMat);
	void StartRendering();
	void EndRendering();

	Window& GetWindow();
	void SetClearColor(const glm::vec4& clearColor);

private:
	void CreateShaders();
	
	Window m_window;
	//http://www.rgbtool.com
	glm::vec4 m_clearColor{ 0.f, 0.f, 0.f, 1.f};
};

#endif
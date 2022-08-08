
#ifndef RENDERER_H
#define RENDERER_H

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include<string>

#include"Helper.h"
#include"Window.h"

struct Mesh;
class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void CreateWindow(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName);
	void InitRenderer();
	void LoadMeshes();

	void RenderMesh(const Mesh& mesh);
	void StartRendering();
	void EndRendering();

	Window& GetWindow();
	void SetClearColor(const glm::vec4& clearColor);

private:
	void CreateShaders();
	
	Window m_window;
	glm::vec4 m_clearColor{0.2f, 0.5f, 0.1f, 1.0f};
};

#endif
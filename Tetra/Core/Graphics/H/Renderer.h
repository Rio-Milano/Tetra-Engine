
#ifndef RENDERER_H
#define RENDERER_H

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include<string>
#include<vector>
#include"../../Core/Structure/H/Mesh.h"
#include"../../Core/Graphics/H/Window.h"
#include"../../OpenGL_Abstraction/H/Shader.h"
#include"../../Core/Structure/H/Camera.h"



class Renderer
{
public:
	//ctors/dtors
	Renderer() = default;
	~Renderer() = default;

	//creates the window
	void CreateWindowGLFW(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName);
	
	//init viewport, set window size change view port resize call back and eneble depth test
	void InitRenderer();
	//render a construct of geometry
	void RenderMesh(const Mesh& mesh, const glm::mat4& worldMat, Shader& shader);
	void RenderTransparentMeshes(const glm::vec3& cameraPosition);
	//set clear color and clear color and depth buffers
	void StartRendering();
	//swap buffers
	void EndRendering();

	//Getters
	Window& GetWindow();
	//Setters
	void SetClearColor(const glm::vec4& clearColor);

private:
	//call back functions
	static void UpdateViewportWhenWindowSizeChanged(GLFWwindow* window, int windowWidth, int windowHeight);
	
	Window m_window;
	//Tool for opengl color picking
	//http://www.rgbtool.com
	glm::vec4 m_clearColor{ 0.f, 0.f, 0.f, 1.f};//default window clear color

	struct MeshDrawCallInfo
	{
		const Mesh* mesh;
		glm::mat4 worldMat;
		Shader* shader;
	};

	std::vector<MeshDrawCallInfo> m_transparentMeshes;
};

#endif
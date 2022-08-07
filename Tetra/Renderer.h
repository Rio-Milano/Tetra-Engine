
#ifndef RENDERER_H
#define RENDERER_H

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include<string>

#include"Helper.h"
#include"Window.h"

class Renderer
{
public:
	Renderer(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName);
	~Renderer() = default;

	void Render();
	Window& GetWindow();

private:
	void Set_GL_Context();
	
	Window m_window;
	Helper_::Mesh_::Mesh m_mesh;
};

#endif
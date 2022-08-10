#include "Renderer.h"
#include"Entity.h"

#include<iostream>
#include<vector>


void Renderer::CreateWindow(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	//create window
	m_window.CreateWindow(renderWindowWidth, renderWindowHeight, renderWindowName);
}

void Renderer::InitRenderer()
{
	int winWidth, winHeight;
	glfwGetWindowSize(m_window.GetWindowPtr(), &winWidth, &winHeight);

	glViewport(0, 0, winWidth, winHeight);

	CreateShaders();

	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderMesh(const Mesh& mesh)
{
	glUseProgram(mesh.m_programID);

	glBindVertexArray(mesh.m_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.m_textureID);

	switch (mesh.m_drawType)
	{
	case 0:
		glDrawElements(GL_TRIANGLES, mesh.m_numberOfElements, GL_UNSIGNED_INT, 0);
		break;
	case 1:
		glDrawArrays(GL_TRIANGLES,  0, mesh.m_triangleCount);
		break;
	default:
		break;
	}


	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}




float
lastMouseX{ 400.0f },
lastMouseY{ 300.0f };


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	lastMouseX = xpos;
	lastMouseY = ypos;
}

float elapsedTime;
float dt;
float fov{ 0 };
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov += yoffset;
}
#include"Camera.h"
Camera cam(90.0f, glm::vec2(400.f, 300.f), 400.f / 300.f, 5.0f, .1f, glm::vec3(0.0f, 0.0f, 3.0f));
void Renderer::SetProjectionViewMatrix(const glm::mat4& viewMatrix, GLFWwindow* window)
{
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	dt = glfwGetTime() - elapsedTime;
	elapsedTime = glfwGetTime();

	ShaderManager.GetShader("main").Use();

	Direction d{Direction::NONE};
	const float cameraVelocity = 4.0f * dt;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		d = Direction::FORWARD;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		d = Direction::BACKWARD;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		d = Direction::LEFT;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		d = Direction::RIGHT;
	cam.Update(dt, fov, lastMouseX, lastMouseY, d);

	fov = 0;
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager.GetShader("main").GetID(), "Projection_X_View"), 1, GL_FALSE, glm::value_ptr(cam.GetPerspectiveViewMat4()));
}

void Renderer::StartRendering()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndRendering()
{
	glfwSwapBuffers(m_window.GetWindowPtr());
}


Window& Renderer::GetWindow()
{
	return m_window;
}

void Renderer::SetClearColor(const glm::vec4& clearColor)
{
	m_clearColor = clearColor;
}

void Renderer::CreateShaders()
{
	Shader shader;
	shader.Create("Data/Shaders/vertexShader.vert", "Data/Shaders/fragmentShader.frag");
	ShaderManager.AddShader("main", shader);

}

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


//
//
//glm::vec3
//	cameraPosition(0.0f, 0.0f, 3.0f),
//	cameraFront(0.0f, 0.0f, -3.0f),//bassed on mouse input normally//need to compute a direction vector bassed on yaw and pitch then normalize for the camera front vector
//	cameraUp(0.0f, 1.0f, 0.0f);
//
//float
//	pitch,
//	yaw{-90.0f},
//	
//	lastMouseX{400.0f},
//	lastMouseY{300.0f};
//
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	float
//		xOffset = xpos - lastMouseX,
//		yOffset = ypos - lastMouseY;
//
//	lastMouseX = xpos;
//	lastMouseY = ypos;
//
//	const float mouseSensitivity{0.1f};
//
//	xOffset *= mouseSensitivity;
//	yOffset *= mouseSensitivity;
//
//	yaw += xOffset;
//	pitch += yOffset;
//
//	if (pitch > 89.0f)
//		pitch = 89.0f;
//	if (pitch < -89.0f)
//		pitch = -89.0f;
//}
//
//float fov{ 90.0f };
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	fov -= (float)yoffset;
//	if (fov < 1.0f)
//		fov = 1.0f;
//	if (fov > 90.0f)
//		fov = 90.0f;
//}
//float elapsedTime;
//float dt;
//
//void Renderer::SetProjectionViewMatrix(const glm::mat4& viewMatrix, const GLuint& programID, GLFWwindow* window)
//{
//	int winWidth, winHeight;
//	glfwGetWindowSize(m_window.GetWindowPtr(), &winWidth, &winHeight);
//	m_projection = glm::perspective(glm::radians(fov), static_cast<float>(winWidth) / static_cast<float>(winHeight), 0.1f, 100.0f);
//
//	glfwSetScrollCallback(window, scroll_callback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetCursorPosCallback(window, mouse_callback);
//
//	dt = glfwGetTime() - elapsedTime;
//	elapsedTime = glfwGetTime();
//
//	glUseProgram(programID);
//	
//	const float cameraVelocity = 4.0f * dt;
//	if (glfwGetKey(window, GLFW_KEY_W))
//		cameraPosition += cameraFront * cameraVelocity;
//	if (glfwGetKey(window, GLFW_KEY_S))
//		cameraPosition -= cameraFront * cameraVelocity;
//	if (glfwGetKey(window, GLFW_KEY_A))
//		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraVelocity;
//	if (glfwGetKey(window, GLFW_KEY_D))
//		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraVelocity;
//
//	glm::vec3 cameraDirection;
//
//	cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	cameraDirection.y = -sin(glm::radians(pitch));
//	cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//
//	cameraFront = glm::normalize(cameraDirection);
//
//	glm::mat4 view;
//	
//	
//	view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp );
//
//	glm::mat4 prespective_x_view = m_projection * view;
//
//	glUniformMatrix4fv(glGetUniformLocation(programID, "Projection_X_View"), 1, GL_FALSE, glm::value_ptr(prespective_x_view));
//}








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
void Renderer::SetProjectionViewMatrix(const glm::mat4& viewMatrix, const GLuint& programID, GLFWwindow* window)
{
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	dt = glfwGetTime() - elapsedTime;
	elapsedTime = glfwGetTime();

	glUseProgram(programID);

	Direction d;
	const float cameraVelocity = 4.0f * dt;
	if (glfwGetKey(window, GLFW_KEY_W))
		d = Direction::FORWARD;
	if (glfwGetKey(window, GLFW_KEY_S))
		d = Direction::BACKWARD;
	if (glfwGetKey(window, GLFW_KEY_A))
		d = Direction::LEFT;
	if (glfwGetKey(window, GLFW_KEY_D))
		d = Direction::RIGHT;
	cam.Update(dt, fov, lastMouseX, lastMouseY, d);

	fov = 0;
	glUniformMatrix4fv(glGetUniformLocation(programID, "Projection_X_View"), 1, GL_FALSE, glm::value_ptr(cam.GetPerspectiveViewMat4()));
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
	//Create Shaders
	const GLuint vertexShaderID{ Helper_::Shaders_::CreateShader(GL_VERTEX_SHADER, "Data/Shaders/vertexShader.vert") };
	const GLuint fragmentShaderID{ Helper_::Shaders_::CreateShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragmentShader.frag") };
	const GLuint programID{ Helper_::Shaders_::CreateProgram(vertexShaderID, fragmentShaderID) };
	Program_Shader_Managment.AddProgram("main", programID);
}

#include "Renderer.h"

#include<iostream>
#include<vector>

#include"Program_Shader_Managment.h"
#define Program_Shader_Managment Program_Shader_Managment::GetInstance()


Renderer::Renderer(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	//initialize glfw
	glfwInit();

	//set opengl context
	Set_GL_Context();

	//create window
	m_window.CreateWindow(renderWindowWidth, renderWindowHeight, renderWindowName);

	//load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize glad!\n";
	else
		std::cout << "GLAD loaded successfully!\n";


	glViewport(0, 0, renderWindowWidth, renderWindowHeight);

	//Create Shaders
	const GLuint vertexShaderID{ Helper_::Shaders_::CreateShader(GL_VERTEX_SHADER, "Data/Shaders/vertexShader.vert") };
	const GLuint fragmentShaderID{ Helper_::Shaders_::CreateShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragmentShader.frag") };
	const GLuint programID{ Helper_::Shaders_::CreateProgram(vertexShaderID, fragmentShaderID) };
	Program_Shader_Managment.AddProgram("main", programID);

	std::vector<glm::vec3> positions
	{
		 glm::vec3(-0.5f, -0.5f, 0.0f),
		 glm::vec3(0.5f, -0.5f, 0.0f),
		 glm::vec3(-0.5f, 0.5f, 0.0f),
		 glm::vec3(0.5f, 0.5f, 0.0f)
	};

	std::vector<glm::vec2> textureCords
	{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f)
	};

	std::vector<GLuint> elements
	{
		0, 1, 2,
		1, 3, 2

	};

	Helper_::Mesh_::GenerateMesh(m_mesh, positions, textureCords, elements, "Data/Images/Bricks.jpg");

	std::cout << "\n\n\n\n\n\n";
	Helper_::Status_::DisplayUsefulInfo();
}


void Renderer::Render()
{
	GLFWwindow* window{ m_window.GetWindowPtr() };

	if (!glfwWindowShouldClose(window))
	{
		glClearColor(0.8f, 0.7f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLuint programID{ Program_Shader_Managment.GetProgramID("main") };
		glUseProgram(programID);

		glUniform1i(glGetUniformLocation(programID, "textureSampler"), 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4 worldMatrix(1.0f);
		worldMatrix = glm::translate(worldMatrix, glm::vec3(0.25f, 0.25f, 0.0f));
		worldMatrix = glm::rotate(worldMatrix, glm::radians((float)glfwGetTime() * 10), glm::vec3(0.0f, 0.0f, 1.0f));
		worldMatrix = glm::scale(worldMatrix, glm::vec3(-0.5f, 0.5f, 0.5f));

		GLint worldMatLoc{ glGetUniformLocation(programID, "worldMat") };
		glUniformMatrix4fv(worldMatLoc, 1, GL_FALSE, glm::value_ptr(worldMatrix));

		Helper_::Mesh_::DrawMesh(m_mesh);

		glfwSwapBuffers(window);
	}
	else
	{
		glfwTerminate();//close glfw
		std::cout << "Goodbye.\n";
	}

}




void Renderer::Set_GL_Context()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//set major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//set minor version to 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//tell opengl to use modern standard
}


Window& Renderer::GetWindow()
{
	return m_window;
}
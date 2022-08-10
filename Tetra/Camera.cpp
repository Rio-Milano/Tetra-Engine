#include "Camera.h"


//
//
//
//
//
//
//float
//lastMouseX{ 400.0f },
//lastMouseY{ 300.0f };
//
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	lastMouseX = xpos;
//	lastMouseY = ypos;
//}
//
//float fov{ 90.0f };
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	fov = yoffset;
//}
//float elapsedTime;
//float dt;
//#include"Camera.h"
//Camera cam(90.0f, glm::vec2(400.f, 300.f), 400.f / 300.f, 5.0f, 0.1f, glm::vec3(0.0f, 0.0f, 3.0f));
//void Renderer::SetProjectionViewMatrix(const glm::mat4& viewMatrix, const GLuint& programID, GLFWwindow* window)
//{
//	glfwSetScrollCallback(window, scroll_callback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetCursorPosCallback(window, mouse_callback);
//
//	dt = glfwGetTime() - elapsedTime;
//	elapsedTime = glfwGetTime();
//
//	glUseProgram(programID);
//
//	Direction d;
//	const float cameraVelocity = 4.0f * dt;
//	if (glfwGetKey(window, GLFW_KEY_W))
//		d = Direction::FORWARD;
//	if (glfwGetKey(window, GLFW_KEY_S))
//		d = Direction::BACKWARD;
//	if (glfwGetKey(window, GLFW_KEY_A))
//		d = Direction::LEFT;
//	if (glfwGetKey(window, GLFW_KEY_D))
//		d = Direction::RIGHT;
//	cam.Update(dt, fov, lastMouseX, lastMouseY, d);
//
//
//	glUniformMatrix4fv(glGetUniformLocation(programID, "Projection_X_View"), 1, GL_FALSE, glm::value_ptr(cam.GetPerspectiveViewMat4()));
//}
//
//
//
//
//




Camera::Camera
(
	const float& fov, 
	const glm::vec2& centreOfViewPort,
	const float& aspectRatio, 
	const float& cameraSpeed, 
	const float& mouseSensitivity, 
	const glm::vec3& cameraPosition
)
	:
		mf_fov(fov),
		mf_aspect_ratio(aspectRatio),
		mf_cameraSpeed(cameraSpeed),
		mf_mouseSensitivity(mouseSensitivity),
		mv3_position(cameraPosition),
		mf_previousMouseX(centreOfViewPort.x),
		mf_previousMouseY(centreOfViewPort.y)
{
}

void Camera::Update(const float& dt, const float& fovZoom, const int& mouseX, const int& mouseY, const Direction& movementDirection)
{
	//changes fov of prespective matrix
	UpdateFOV(fovZoom);

	//updates pitch and yaw values
	MouseInput(dt, mouseX, mouseY);

	//calculate forward vector using new pitch and yaw and to calculate right vector later
	UpdateForwardVector();

	//update right vector so it can be used by keyboard input
	UpdateRightVector();

	//process all user input
	KeyboardInput(dt, movementDirection);

	ComputeViewMatrix();
}

glm::mat4 Camera::GetPerspectiveViewMat4() const
{
	return mm4_projectionMat * mm4_viewMat;
}


void Camera::ComputePrespectiveMatrix()
{
	mm4_projectionMat = glm::perspective(glm::radians(mf_fov), mf_aspect_ratio, mf_nearPlane, mf_farPlane);
}

void Camera::ComputeViewMatrix()
{
	mm4_viewMat = glm::lookAt(mv3_position, mv3_position + mv3_forward, mv3_up);
}
void Camera::UpdateFOV(const float& fovZoom)
{
	mf_fov -=fovZoom;
	if (mf_fov < 1.0f)
		mf_fov = 1.0f;
	else if (mf_fov > 90.0f)
		mf_fov = 90.0f;
	ComputePrespectiveMatrix();//now that the fov has changed we must recalculate the projection matrix
}


void Camera::MouseInput(const float& dt, const int& mouseX, const int& mouseY)
{
	//calculate amount mouse moved on xy plane
	float
		lf_mouseXOffset{ mouseX - mf_previousMouseX },
		lf_mouseYOffset{ mf_previousMouseY - mouseY };

	
	//update previous mouse position
	mf_previousMouseX = mouseX;
	mf_previousMouseY = mouseY;

	//apply sensitivity of mouse to displacement
	lf_mouseXOffset *= mf_mouseSensitivity;
	lf_mouseYOffset *= mf_mouseSensitivity;

	//add displacement to pitch and yaw respectively
	mf_pitch += lf_mouseYOffset;
	mf_yaw += lf_mouseXOffset;

	//constrain pitch and yaw to range
	if (mf_pitch > 89.f)
		mf_pitch = 89.f;

	else if (mf_pitch < -89.f)
		mf_pitch = -89.f;

	if (mf_yaw < -360.f)
		mf_yaw = -360.f;
	else if (mf_yaw > 360.f)
		mf_yaw = 360.f;



}

void Camera::UpdateForwardVector()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(mf_yaw)) * cos(glm::radians(mf_pitch));
	direction.y = sin(glm::radians(mf_pitch));
	direction.z = sin(glm::radians(mf_yaw)) * cos(glm::radians(mf_pitch));

	mv3_forward = glm::normalize(direction);
}

void Camera::UpdateRightVector()
{
	mv3_right = glm::normalize(glm::cross(mv3_forward, mv3_up));
}

void Camera::KeyboardInput(const float& dt, const Direction& movementDirection)
{
	if (movementDirection == Direction::FORWARD)
	{
		mv3_position += mv3_forward * mf_cameraSpeed * dt;
	}

	if (movementDirection == Direction::BACKWARD)
	{
		mv3_position -= mv3_forward * mf_cameraSpeed * dt;
	}

	if (movementDirection == Direction::LEFT)
	{
		mv3_position -= mv3_right * mf_cameraSpeed * dt;
	}

	if (movementDirection == Direction::RIGHT)
	{
		mv3_position += mv3_right * mf_cameraSpeed * dt;
	}
}




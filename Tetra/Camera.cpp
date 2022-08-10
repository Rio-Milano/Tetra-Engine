#include "Camera.h"

#include"InputManager.h"
#define InputManager InputManager::GetInstance()


void Camera::Initialize
(
	const float& fov,
	const glm::vec2& centreOfViewPort,
	const float& aspectRatio,
	const float& cameraSpeed,
	const float& mouseSensitivity,
	const glm::vec3& cameraPosition,
	GLFWwindow* windowPtr
)
{
	mf_fov = fov;
	mf_aspect_ratio = aspectRatio;
	mf_cameraSpeed = cameraSpeed;
	mf_mouseSensitivity = mouseSensitivity;
	mv3_position = cameraPosition;
	m_windowPtr = windowPtr;

	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Camera::Update(const float& dt)
{
	//changes fov of prespective matrix
	UpdateFOV();

	//updates pitch and yaw values
	MouseInput(dt);

	//calculate forward vector using new pitch and yaw and to calculate right vector later
	UpdateForwardVector();

	//update right vector so it can be used by keyboard input
	UpdateRightVector();

	//process all user input
	KeyboardInput(dt);

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
void Camera::UpdateFOV()
{
	mf_fov -= InputManager.m_mouseScrollY;
	InputManager.m_mouseScrollY = 0;

	if (mf_fov < 1.0f)
		mf_fov = 1.0f;
	else if (mf_fov > 90.0f)
		mf_fov = 90.0f;
	ComputePrespectiveMatrix();//now that the fov has changed we must recalculate the projection matrix
}


void Camera::MouseInput(const float& dt)
{

	float 
		mouseX = InputManager.m_currentMousePosition.x,
		mouseY = InputManager.m_currentMousePosition.y,

	//calculate amount mouse moved on xy plane

		lf_mouseXOffset{ mouseX - mf_previousMouseX },
		lf_mouseYOffset{ mf_previousMouseY - mouseY };

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

	/*if (mf_yaw < -360.f)
		mf_yaw = -360.f;
	else if (mf_yaw > 360.f)
		mf_yaw = 360.f;*/



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

void Camera::KeyboardInput(const float& dt)
{

	if (glfwGetKey(m_windowPtr, GLFW_KEY_W) == GLFW_PRESS)
		mv3_position += mv3_forward * mf_cameraSpeed * dt;

	if (glfwGetKey(m_windowPtr, GLFW_KEY_S) == GLFW_PRESS)
		mv3_position -= mv3_forward * mf_cameraSpeed * dt;

	if (glfwGetKey(m_windowPtr, GLFW_KEY_A) == GLFW_PRESS)
		mv3_position -= mv3_right * mf_cameraSpeed * dt;

	if (glfwGetKey(m_windowPtr, GLFW_KEY_D) == GLFW_PRESS)
		mv3_position += mv3_right * mf_cameraSpeed * dt;

}




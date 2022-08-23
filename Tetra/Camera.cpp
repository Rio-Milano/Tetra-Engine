#include"XInput_Wrapper.h"

#include "Camera.h"
#include"InputManager.h"
#define InputManager InputManager::GetInstance()
#include<iostream>
Camera::~Camera()
{
	delete controller;
};
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
	mf_previousMouseX = centreOfViewPort.x;
	mf_previousMouseY = centreOfViewPort.y;

	if(m_fpsCamera)
		std::cout << "FPS camera enabled!\n";

	glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	controller = new XInput_Wrapper;
}

void Camera::Update(const float& dt)
{
	controller->Update();

	if (m_usingCamera)
	{
		if (glfwGetKey(m_windowPtr, GLFW_KEY_U) == GLFW_PRESS)
		{
			glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_usingCamera = false;
		}
	}
	else
	{
		if (glfwGetKey(m_windowPtr, GLFW_KEY_F) == GLFW_PRESS)
		{
			glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_usingCamera = true;
		}

	}

	//changes fov of prespective matrix
	UpdateFOV();

	//updates pitch and yaw values
	MouseInput(dt);

	std::shared_ptr<Controller> controller0 = controller->GetController(0);
	mf_pitch += controller0->GetRightStickY() * 2.f;
	mf_yaw += controller0->GetRightStickX() * 2.f;

	//calculate forward vector using new pitch and yaw and to calculate right vector later
	UpdateForwardVector();

	//update right vector so it can be used by keyboard input
	UpdateRightVector();

	//process all user input
	KeyboardInput(dt);

	mv3_position += (mv3_forward * controller0->GetLeftStickY() + mv3_right * controller0->GetLeftStickX()) * mf_cameraSpeed * dt;

	ComputeViewMatrix();
}

glm::mat4 Camera::GetPerspectiveViewMat4() const
{
	return mm4_projectionMat * mm4_viewMat;
}

const glm::vec3& Camera::GetPosition() const
{
	return mv3_position;
}

const glm::vec3& Camera::GetForwardVector() const
{
	return mv3_forward;
}

const bool& Camera::GetUsingCamera() const
{
	return m_usingCamera;
}

void Camera::ComputePrespectiveMatrix()
{
	mm4_projectionMat = glm::perspective(glm::radians(mf_fov), mf_aspect_ratio, mf_nearPlane, mf_farPlane);
}

void Camera::ComputeViewMatrix()
{
	if(m_fpsCamera)
		mv3_position.y = 0;//mimics a fps camera
	
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
		mouseY = InputManager.m_currentMousePosition.y;

	if (InputManager.m_firstMouseEvent)
	{
		if (!InputManager.m_seccondMouseEvent)
		{
			mf_previousMouseX = mouseX;
			mf_previousMouseY = mouseY;
		}
		//calculate amount mouse moved on xy plane
		float
			lf_mouseXOffset{ mouseX - mf_previousMouseX },
			lf_mouseYOffset{ mf_previousMouseY - mouseY };

		mf_previousMouseX = mouseX;
		mf_previousMouseY = mouseY;

		//apply sensitivity of mouse to displacement
		lf_mouseXOffset *= mf_mouseSensitivity;
		lf_mouseYOffset *= mf_mouseSensitivity;


		//add displacement to pitch and yaw respectively
		if (m_usingCamera == false)
		{
			if (glfwGetMouseButton(m_windowPtr, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				mf_pitch += lf_mouseYOffset;
				mf_yaw += lf_mouseXOffset;
			}
			else
				glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			mf_pitch += lf_mouseYOffset;
			mf_yaw += lf_mouseXOffset;
		}



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

}

void Camera::UpdateForwardVector()
{
	glm::vec3 direction(0.0f);
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

	glm::vec3 direction(0.0f);

	if (glfwGetKey(m_windowPtr, GLFW_KEY_W) == GLFW_PRESS)
		direction += mv3_forward;

	if (glfwGetKey(m_windowPtr, GLFW_KEY_S) == GLFW_PRESS)
		direction -= mv3_forward;

	if (glfwGetKey(m_windowPtr, GLFW_KEY_A) == GLFW_PRESS)
		direction -= mv3_right;

	if (glfwGetKey(m_windowPtr, GLFW_KEY_D) == GLFW_PRESS)
		direction += mv3_right;

	if(glm::dot(direction, direction) > 0)
		direction = glm::normalize(direction) * mf_cameraSpeed * dt;
	
	glm::vec3 prevPos = mv3_position;
	mv3_position += direction;

}




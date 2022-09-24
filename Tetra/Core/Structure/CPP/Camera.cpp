#include"../../Wrapper/H/XInput_Wrapper.h"

#include "../H/Camera.h"
#include"../../Managers/H/InputManager.h"
#define InputManager InputManager::GetInstance()
#include<iostream>
#include<assert.h>

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

	/*
	automatically unfocuses camera to use mouse when in 
	debug so primary monitor can be used for inspecting break points
	*/
	#if(_DEBUG)
		m_usingCamera = false;
		glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	#else
		m_usingCamera = true;
		glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	#endif

	//assign a controller
	controller = new XInput_Wrapper;
}

void Camera::Update(const float& dt)
{
	//update user input from controller
	controller->Update();

	//if using the camera
	if (m_usingCamera)
	{
		//check for camera unfocus
		if (glfwGetKey(m_windowPtr, GLFW_KEY_U) == GLFW_PRESS)
		{
			//make cursor seen
			glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//now not using camera
			m_usingCamera = false;
		}
	}
	else
	{
		//if not using camera 

		//check for camera focus
		if (glfwGetKey(m_windowPtr, GLFW_KEY_F) == GLFW_PRESS)
		{
			//disable cursor 
			glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			//now using camera
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

	//update right vector by using up and forward so it can be used by keyboard input
	UpdateRightVector();

	//process all keyboard user input
	KeyboardInput(dt);

	//process controller input
	mv3_position += (mv3_forward * controller0->GetLeftStickY() + mv3_right * controller0->GetLeftStickX()) * mf_cameraSpeed * dt;
	//compute view matrix from camera directions
	ComputeViewMatrix();
}

glm::mat4 Camera::GetPerspectiveViewMat4() const
{
	return mm4_projectionMat * mm4_viewMat;
}

glm::mat4 Camera::GetViewMat4()const
{
	return mm4_viewMat;
};

glm::mat4 Camera::GetPerspectiveMat4()const
{
	return mm4_projectionMat;
};

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
	//apply any fov changes to the fov
	mf_fov -= InputManager.m_mouseScrollY;
	//reset scroll y of mouse
	InputManager.m_mouseScrollY = 0;

	//constrain fov
	if (mf_fov < 1.0f)
		mf_fov = 1.0f;
	else if (mf_fov > 90.0f)
		mf_fov = 90.0f;
	//compute perspective matrix
	ComputePrespectiveMatrix();//now that the fov has changed we must recalculate the projection matrix
}

void Camera::MouseInput(const float& dt)
{
	//get mouse position
	float
		mouseX = InputManager.m_currentMousePosition.x,
		mouseY = InputManager.m_currentMousePosition.y;
	
	//if the mouse has moved
	if (InputManager.m_firstMouseEvent)
	{
		//if the mouse has moved only once
		if (!InputManager.m_seccondMouseEvent)
		{
			//set mouse previous values
			mf_previousMouseX = mouseX;
			mf_previousMouseY = mouseY;
		}
		//calculate amount mouse moved on xy plane
		float
			lf_mouseXOffset{ mouseX - mf_previousMouseX },
			lf_mouseYOffset{ mf_previousMouseY - mouseY };

		//set previous mouse to current mouse possition
		mf_previousMouseX = mouseX;
		mf_previousMouseY = mouseY;

		//apply sensitivity of mouse to displacement
		lf_mouseXOffset *= mf_mouseSensitivity;
		lf_mouseYOffset *= mf_mouseSensitivity;


		//add displacement to pitch and yaw respectively
		
		//if not using camera
		if (m_usingCamera == false)
		{
			//we must click and hold mouse right to move
			if (glfwGetMouseButton(m_windowPtr, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				//when moving mouse disable the cursor
				glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				//apply angle changes
				mf_pitch += lf_mouseYOffset;
				mf_yaw += lf_mouseXOffset;
			}
			else
				//if not using mouse and not holding down left mouse then enable cursor
				glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			//if using camera then apply values as normal
			mf_pitch += lf_mouseYOffset;
			mf_yaw += lf_mouseXOffset;
		}



		//constrain pitch and yaw to range
		if (mf_pitch > 89.f)
			mf_pitch = 89.f;

		else if (mf_pitch < -89.f)
			mf_pitch = -89.f;
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

	if (glm::dot(direction, direction) > 0)
	{
		direction = glm::normalize(direction) * mf_cameraSpeed * dt;
		if (glfwGetKey(m_windowPtr, GLFW_KEY_LEFT_SHIFT))
			direction *= 4.f;
	}
	
	glm::vec3 prevPos = mv3_position;
	mv3_position += direction;

}




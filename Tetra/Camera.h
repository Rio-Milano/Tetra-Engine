#ifndef CAMERA_H
#define CAMERA_H

#include<GLFW/glfw3.h>

#include"glmIncludes.h"

enum Direction
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera() = default;

	void Initialize
	(
		const float& fov,
		const glm::vec2& centreOfViewPort,
		const float& aspectRatio,
		const float& cameraSpeed,
		const float& mouseSensitivity,
		const glm::vec3& cameraPosition,
		GLFWwindow* windowPtr
	);

	void Update(const float& dt);
	glm::mat4 GetPerspectiveViewMat4()const;
	const glm::vec3& GetPosition()const;
	const glm::vec3& GetForwardVector()const;
	const bool& GetUsingCamera()const;


private:
	float
		//Projection
		mf_fov,
		mf_aspect_ratio,
		mf_nearPlane{.1f},
		mf_farPlane{100.f},

		//View
		mf_pitch,
		mf_yaw{-89.f},
		mf_cameraSpeed,
		mf_mouseSensitivity,
		mf_previousMouseX,
		mf_previousMouseY;

	glm::vec3
		//View
		mv3_position,
		mv3_forward,
		mv3_right,
		mv3_up{.0f, 1.f, .0f};

	glm::mat4 
		mm4_viewMat,
		mm4_projectionMat;

	bool
		m_fpsCamera{ false },
		m_usingCamera{true};

	GLFWwindow* m_windowPtr;


	void ComputePrespectiveMatrix();
	void ComputeViewMatrix();

	void UpdateFOV();
	void KeyboardInput(const float& dt);
	void MouseInput(const float& dt);
	void UpdateForwardVector();
	void UpdateRightVector();

	
};

#endif


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
	explicit Camera
	(
		const float& fov,
		const glm::vec2& centreOfViewPort,
		const float& aspectRatio,
		const float& cameraSpeed,
		const float& mouseSensitivity,
		const glm::vec3& cameraPosition
	);

	void Update(const float& dt, const float& fovZoom, const int& mouseX, const int& mouseY, const Direction& movementDirection);
	glm::mat4 GetPerspectiveViewMat4()const;
	

private:
	float
		//Projection
		mf_fov,
		mf_aspect_ratio,
		mf_nearPlane{.1f},
		mf_farPlane{100.f},

		//View
		mf_pitch,
		mf_yaw{-90.f},
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
		mb_usePrespectiveProjection;


	void ComputePrespectiveMatrix();
	void ComputeViewMatrix();

	void UpdateFOV(const float& fovZoom);
	void KeyboardInput(const float& dt, const Direction& movementDirection);
	void MouseInput(const float& dt, const int& mouseX, const int& mouseY);
	void UpdateForwardVector();
	void UpdateRightVector();

	
};

#endif


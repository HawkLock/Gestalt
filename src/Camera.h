#pragma once

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "PhysicsObject.h"

// Default values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float FOV = 45;
const float SCROLLSPEED = 20.0f;

class Camera
{
protected:
	GLFWwindow* window;
	float yaw;
	float pitch;
	float defaultX = 400, defaultY = 300;
	float currX = defaultX, currY = defaultY;
	const float sensitivity = 0.1f;

	// For focus object orbiting
	PhysicsObject* focusObject;
	bool followFocusedObject = false;
	const float defaultOrbitRadius = 5;
	const glm::vec3 defaultOrbitOffset = glm::vec3(0, 0, 1) * defaultOrbitRadius;
	const float minOrbitRadius = 1;

	float orbitRadius = defaultOrbitRadius;
	glm::vec3 orbitOffset = defaultOrbitOffset;

public:
	Camera();

	float fov;
	float ScrollSpeed;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	glm::mat4 GetCameraView();
	void SetDefaultCursorPos(float x, float y) { defaultX = x; defaultY = y; currX = defaultX, currY = defaultY; }
	void ProcessMouseMovement(double xpos, double ypos);
	void ProcessMouseScroll(float yoffset, float DeltaTime);
	void UpdateCameraVectors();

	void CameraUpdate(bool shouldFollow);
	void UpdateFocusObject(PhysicsObject* obj);
};
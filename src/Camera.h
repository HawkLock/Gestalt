#pragma once
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

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

public:
	Camera();

	float fov;
	float ScrollSpeed;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	glm::mat4 GetCameraView();
	void SetDefaultCursorPos(float x, float y) { defaultX = x; defaultY = y; }
	void ProcessMouseMovement(double xpos, double ypos);
	void ProcessMouseScroll(float yoffset, float DeltaTime);
	void UpdateCameraVectors();
};
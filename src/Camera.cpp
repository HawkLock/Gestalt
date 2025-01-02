#include "Camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = YAW;
	pitch = PITCH;
	fov = FOV;
	ScrollSpeed = SCROLLSPEED;

	UpdateCameraVectors();
}

glm::mat4 Camera::GetCameraView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

// Default position is in the middle of the screen
void Camera::ProcessMouseMovement(double xpos, double ypos)
{
	float xoffset = xpos - currX;
	float yoffset = currY - ypos;
	currX = xpos;
	currY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	if (!followFocusedObject) {

		UpdateCameraVectors();
	}
	else {

		// Calculate new orbitOffset using spherical coordinates
		orbitOffset.x = orbitRadius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		orbitOffset.y = orbitRadius * sin(glm::radians(pitch));
		orbitOffset.z = orbitRadius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		UpdateCameraVectors();
	}
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void Camera::ProcessMouseScroll(float yoffset, float DeltaTime)
{
	// Normal Behavior
	if (!followFocusedObject) {
		cameraPos += (float)yoffset * cameraFront;
	}
	else {
		orbitRadius -= (float)yoffset;
		if (orbitRadius < minOrbitRadius) {
			orbitRadius = minOrbitRadius;
		}
	}
}

void Camera::CameraUpdate(bool shouldFollow) {
	followFocusedObject = shouldFollow;
	if (focusObject == nullptr || !followFocusedObject) {
		return;
	}
	orbitOffset = glm::normalize(orbitOffset) * orbitRadius;

	cameraPos = focusObject->pos + orbitOffset; // Position
	cameraFront = glm::normalize(focusObject->pos - cameraPos);
}


void Camera::UpdateFocusObject(PhysicsObject* obj) {
	focusObject = obj;
	if (obj == nullptr) {
		orbitRadius = defaultOrbitRadius;
	}
}

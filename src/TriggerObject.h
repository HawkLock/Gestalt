#pragma once
#include <glm/glm/glm.hpp>
#include <iostream>
#include <vector>
#include <stack>
#include <chrono>

#include "Mesh.h"
#include "RenderUtils.h"

class TriggerObject {

protected:

	float sideLength = 1;

	Mesh Model;

public:
	void (*triggerPtr)(std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point);

	glm::vec3 pos;
	glm::quat rot;

	glm::vec3 acceleration;
	glm::vec3 velocity; // In m/s
	glm::vec3 angularAcceleration;
	glm::vec3 angularVelocity;

	float scale = 1;
	std::string modelPath;
	std::string texturePath;

	TriggerObject(glm::vec3 initialPosition, glm::quat initialRot, float faceSize, std::string& modelPath, std::string &texturePath);

	// Getters and Setters
	glm::vec3 GetCurrentPos() { return pos; }
	glm::quat GetCurrentRot() { return rot; }

	glm::vec3 GetCurrentVelocity() { return velocity; }
	glm::vec3 GetCurrentAngularVelocity() { return angularVelocity; }
	glm::vec3 GetCurrentNetForce() { return glm::vec3(); }

	Mesh& GetMesh() { return Model; }
	float GetFaceExtent() { return Model.faceLength; }

	void SetPos(glm::vec3 newPos) { pos = newPos; }
	void SetRot(glm::quat newRot) { rot = newRot; }

	void SetVelocity(glm::vec3 newVelocity) { velocity = newVelocity; }
	void SetAngularVelocity(glm::vec3 newVelocity) { angularVelocity = newVelocity; }

	void AddRotation(glm::quat addRot) { rot = glm::normalize(addRot * rot); }

	void ScaleSize(float scale);

	// Functionality
	void setTrigger(void (*func)(std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point)) { triggerPtr = func; }

	void Trigger(std::chrono::steady_clock::time_point cTime, std::chrono::steady_clock::time_point sTime) { triggerPtr(cTime, sTime); }

	// Collision
	std::vector<Vertex> extractVertices(const std::vector<float>& data);

	std::vector<glm::vec3> GetVertices();

	void RenderMesh(const Shader& shader);

private:

};
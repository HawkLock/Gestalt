#pragma once
#include <glm/glm/glm.hpp>

// Currently only a cube
class Mesh {

public:
	glm::vec3 pos;
	float faceLength;
	float vertices[180];

	Mesh() {};
	Mesh(glm::vec3 initialPosition, float faceSize);

	void Move(glm::vec3 newPos) { pos = newPos; }
};
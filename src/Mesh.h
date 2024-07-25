#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>

// Currently only a cube
class Mesh {

public:
	glm::vec3 pos;
	float faceLength;
	std::vector<float> vertices;
	size_t vertexCount;

	unsigned int VAO, VBO;

	Mesh(glm::vec3 initialPosition, float faceSize);

	void SetupMesh();
	void Bind() const { glBindVertexArray(VAO); }

	void Move(glm::vec3 newPos) { pos = newPos; }
	void ChangeSize(float faceSize);

	int GetVertexCount() { return vertexCount; }

	~Mesh();
};
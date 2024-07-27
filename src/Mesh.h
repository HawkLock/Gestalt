#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include "Shader.h"

struct VertexData {
    float x, y, z;   // Position
    float u, v;      // UV coordinates
    float nx, ny, nz; // Normal
};

struct Vertex {
	float x, y, z;   // Position
	float u, v;      // UV coordinates
};

struct Normal {
	float nx, ny, nz; // Normal
};

// Currently only a cube
class Mesh {

public:
	glm::vec3 pos;
	float faceLength;
	std::vector<float> verticesU;
	std::vector<Vertex> vertices;
	std::vector<Normal> normals;
	std::vector<unsigned int> indices;
	size_t vertexCount;

	unsigned int VAO, VBO, EBO;

	Mesh(glm::vec3 initialPosition, float faceSize, std::vector<float> inputVertices);
	Mesh(glm::vec3 initialPosition, float faceSize, std::string &modelPath);

	void SetupMeshU();
	void SetupMesh();
	void Bind() const { glBindVertexArray(VAO); }

    // Function to parse vertex data into two separate containers
	void parseVertexData(const std::string& filepath, std::vector<Vertex>& vertices, std::vector<Normal>& normals, std::vector<unsigned int>& indices);
	void Move(glm::vec3 newPos) { pos = newPos; }
	void ChangeSize(float scale);

	int GetVertexCount() { return vertexCount; }

	~Mesh();
};
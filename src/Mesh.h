#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>

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

struct Face {
	std::vector<unsigned int> vertexIndices; // Indices of vertices that form the face
};

// Currently only a cube
class Mesh {

public:
	glm::vec3 pos;
	float faceLength;
	std::vector<float> verticesU;
	std::vector<Vertex> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> edges;
	size_t vertexCount;

	unsigned int VAO, VBO, EBO;

	Mesh(glm::vec3 initialPosition, float faceSize, std::vector<float> inputVertices);
	Mesh(glm::vec3 initialPosition, float faceSize, std::string &modelPath);

	void SetupMeshU();
	void SetupMesh();
	void Bind() const { glBindVertexArray(VAO); }

    // Function to parse vertex data into two separate containers
	void parseVertexData(const std::string& filepath, std::vector<Vertex>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices, std::vector<glm::vec3>& edgeVectors);

	void Move(glm::vec3 newPos) { pos = newPos; }
	void ChangeSize(float scale);

	int GetVertexCount() { return vertexCount; }

    // Helper function to create a unique edge identifier (sorted vertex indices)
	std::pair<unsigned int, unsigned int> makeEdge(unsigned int v1, unsigned int v2);

    // Function to extract edges from indices
	void extractEdgesFromIndices(
		const std::vector<unsigned int>& indices,
		const std::vector<Vertex>& vertices,
		std::vector<glm::vec3>& edges
	);
	~Mesh();
};
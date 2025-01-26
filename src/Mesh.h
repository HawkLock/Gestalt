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
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "TextureUtils.h"

struct VertexData {
    float x, y, z;   // Position
    float u, v;      // UV coordinates
    float nx, ny, nz; // Normal
};

struct Vertex {
	float x, y, z;   // Position
	float u, v;      // UV coordinates
};

// Currently only a cube
class Mesh {

public:
	glm::vec3 pos;
	float faceLength;
	std::vector<float> verticesU;
	std::vector<Vertex> vertices;
	std::vector<Vertex> originalVertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> edges;
	size_t vertexCount;

	unsigned int VAO, VBO, EBO;

	GLuint texture;

	Mesh(glm::vec3 initialPosition, float faceSize, std::vector<float> inputVertices);
	Mesh(glm::vec3 initialPosition, float faceSize, std::string &modelPath, std::string &texturePath);

	void SetupMeshU();
	void SetupMesh();
	void Bind() const { glBindVertexArray(VAO); }

    // Function to parse vertex data into two separate containers
	void parseVertexData(const std::string& filepath, std::vector<Vertex>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices, std::vector<glm::vec3>& edgeVectors);

	void Move(glm::vec3 newPos) { pos = newPos; }
	void ChangeSize(float scale);
	void ChangeSizeFromOriginal(float scale);
	void ChangeSizeFromOriginalSingleDimension(float scale, char dimension);

	int GetVertexCount() { return vertexCount; }
	std::vector<glm::vec3> GetVertexPositions();

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
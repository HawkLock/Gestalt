#pragma once
#include "Mesh.h"

Mesh::Mesh(glm::vec3 initialPosition, float faceSize, std::vector<float> inputVertices)
{
	pos = initialPosition;
	faceLength = faceSize;

	// Generate the vertices for the cube
    float verticeDistance = faceLength / 2;
    verticesU = inputVertices;

    SetupMeshU();

    vertexCount = verticesU.size() / 5; // There are five components to each in the vector

}

Mesh::Mesh(glm::vec3 initialPosition, float faceSize, std::string &modelPath)
{
    pos = initialPosition;
    faceLength = faceSize;

    // Generate the vertices for the cube
    float verticeDistance = faceLength / 2;

    vertices = std::vector<Vertex>();
    parseVertexData(modelPath, vertices, normals, indices);

    SetupMesh();
    vertexCount = vertices.size(); // There are five components to each in the vector
}

void Mesh::SetupMeshU()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    std::cout << "VBO: " << VBO << std::endl;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesU.size() * sizeof(float), &verticesU[0], GL_STATIC_DRAW);

    // Assuming each vertex has 3 position floats and 2 texture floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind VBO and upload vertex data (positions and UVs)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex) + normals.size() * sizeof(Normal), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), normals.size() * sizeof(Normal), normals.data());

    // Bind EBO and upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Setup vertex attributes
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);
    // UV attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(1);

    // Setup normal attributes
    // Assuming normals are stored after the vertices in the buffer
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}



void Mesh::parseVertexData(const std::string& filepath, std::vector<Vertex>& vertices, std::vector<Normal>& normals,
    std::vector<unsigned int>& indices) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        unsigned int index;
        Vertex vertex;
        Normal normal;
        if (!(iss >> index >> vertex.x >> vertex.y >> vertex.z >> vertex.u >> vertex.v >> normal.nx >> normal.ny >> normal.nz)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }

        // Assuming that vertex and normal data are stored in the order they are read
        if (index >= vertices.size()) {
            vertices.resize(index + 1);
            normals.resize(index + 1);
        }

        vertices[index] = vertex;
        normals[index] = normal;
        indices.push_back(index);
    }

    file.close();
}


void Mesh::ChangeSize(float scale)
{
    // Generate the vertices for the cube
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].x *= scale;     // Scale x
        vertices[i].y *= scale;   // Scale y
        vertices[i].z *= scale;   // Scale z
        // Skip vertices[i+3] (u) and vertices[i+4] (v) as they are texture coordinates
    }

    SetupMesh();
}


Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
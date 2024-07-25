#pragma once
#include "Mesh.h"

Mesh::Mesh(glm::vec3 initialPosition, float faceSize)
{
	pos = initialPosition;
	faceLength = faceSize;

	// Generate the vertices for the cube
    float verticeDistance = faceLength / 2;
    vertices =  {
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 0.0f,
         verticeDistance, -verticeDistance, -verticeDistance,  1.0f, 0.0f,
         verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
         verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
        -verticeDistance,  verticeDistance, -verticeDistance,  0.0f, 1.0f,
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 0.0f,

        -verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,
         verticeDistance, -verticeDistance,  verticeDistance,  1.0f, 0.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 1.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 1.0f,
        -verticeDistance,  verticeDistance,  verticeDistance,  0.0f, 1.0f,
        -verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,

        -verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,
        -verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
        -verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,
        -verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,

         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,
         verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
         verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
         verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
         verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,

        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
         verticeDistance, -verticeDistance, -verticeDistance,  1.0f, 1.0f,
         verticeDistance, -verticeDistance,  verticeDistance,  1.0f, 0.0f,
         verticeDistance, -verticeDistance,  verticeDistance,  1.0f, 0.0f,
        -verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,

        -verticeDistance,  verticeDistance, -verticeDistance,  0.0f, 1.0f,
         verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,
        -verticeDistance,  verticeDistance,  verticeDistance,  0.0f, 0.0f,
        -verticeDistance,  verticeDistance, -verticeDistance,  0.0f, 1.0f
    };

    SetupMesh();
    vertexCount = vertices.size() / 5; // There are five components to each in the vector

}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Assuming each vertex has 3 position floats and 2 texture floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


void Mesh::ChangeSize(float faceSize)
{
    faceLength = faceSize;

    // Generate the vertices for the cube
    float verticeDistance = faceLength / 2;
    vertices = {
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 0.0f,
         verticeDistance, -verticeDistance, -verticeDistance,  1.0f, 0.0f,
         verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
         verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
        -verticeDistance,  verticeDistance, -verticeDistance,  0.0f, 1.0f,
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 0.0f,

        -verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,
         verticeDistance, -verticeDistance,  verticeDistance,  1.0f, 0.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 1.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 1.0f,
        -verticeDistance,  verticeDistance,  verticeDistance,  0.0f, 1.0f,
        -verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,

        -verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,
        -verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
        -verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,
        -verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,

         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,
         verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
         verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
         verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
         verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,

        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,
         verticeDistance, -verticeDistance, -verticeDistance,  1.0f, 1.0f,
         verticeDistance, -verticeDistance,  verticeDistance,  1.0f, 0.0f,
         verticeDistance, -verticeDistance,  verticeDistance,  1.0f, 0.0f,
        -verticeDistance, -verticeDistance,  verticeDistance,  0.0f, 0.0f,
        -verticeDistance, -verticeDistance, -verticeDistance,  0.0f, 1.0f,

        -verticeDistance,  verticeDistance, -verticeDistance,  0.0f, 1.0f,
         verticeDistance,  verticeDistance, -verticeDistance,  1.0f, 1.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,
         verticeDistance,  verticeDistance,  verticeDistance,  1.0f, 0.0f,
        -verticeDistance,  verticeDistance,  verticeDistance,  0.0f, 0.0f,
        -verticeDistance,  verticeDistance, -verticeDistance,  0.0f, 1.0f
    };

    SetupMesh();

}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
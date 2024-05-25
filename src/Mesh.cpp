#pragma once
#include "Mesh.h"

Mesh::Mesh(glm::vec3 initialPosition, float faceSize)
{
	pos = initialPosition;
	faceLength = faceSize;

	// Generate the vertices for the cube
    float verticeDistance = faceLength / 2;
    float newVertices[180] = {
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

    for (int i = 0; i < 180; i++)
    {
        vertices[i] = newVertices[i];
    }
}
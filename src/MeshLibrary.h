#pragma once
#include <vector>

class MeshLibrary {

public:
	enum Shapes {Cube, Pyramid};

	static std::vector<float> getCubeVertices(float verticeDistance) {
		return {
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
	}

    static std::vector<float> getPyramidVertices(float verticeDistance) {
        return {
    // Base of the pyramid (square)
    -verticeDistance, 0.0f, -verticeDistance, 0.0f, 0.0f,
     verticeDistance, 0.0f, -verticeDistance, 1.0f, 0.0f,
     verticeDistance, 0.0f,  verticeDistance, 1.0f, 1.0f,
     verticeDistance, 0.0f,  verticeDistance, 1.0f, 1.0f,
    -verticeDistance, 0.0f,  verticeDistance, 0.0f, 1.0f,
    -verticeDistance, 0.0f, -verticeDistance, 0.0f, 0.0f,

    // Side 1 (triangle)
    -verticeDistance, 0.0f, -verticeDistance, 0.0f, 0.0f,
     verticeDistance, 0.0f, -verticeDistance, 1.0f, 0.0f,
     0.0f, 2 * verticeDistance, 0.0f, 0.5f, 1.0f,

    // Side 2 (triangle)
     verticeDistance, 0.0f, -verticeDistance, 0.0f, 0.0f,
     verticeDistance, 0.0f,  verticeDistance, 1.0f, 0.0f,
     0.0f, 2 * verticeDistance, 0.0f, 0.5f, 1.0f,

    // Side 3 (triangle)
     verticeDistance, 0.0f,  verticeDistance, 0.0f, 0.0f,
    -verticeDistance, 0.0f,  verticeDistance, 1.0f, 0.0f,
     0.0f, 2 * verticeDistance, 0.0f, 0.5f, 1.0f,

    // Side 4 (triangle)
    -verticeDistance, 0.0f,  verticeDistance, 0.0f, 0.0f,
    -verticeDistance, 0.0f, -verticeDistance, 1.0f, 0.0f,
     0.0f, 2 * verticeDistance, 0.0f, 0.5f, 1.0f
};
    }

};
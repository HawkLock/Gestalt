#pragma once
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <vector>
#include <string>
#include <glm/glm/glm.hpp>

class PhysicsUtility {

public: 

	static struct Edge {
		glm::vec3 Vertex1;
		glm::vec3 Vertex2;

		Edge() {
			Vertex1 = glm::vec3(0.f);
			Vertex2 = glm::vec3(0.f);
		}

		Edge(glm::vec3 vert1, glm::vec3 vert2) {
			Vertex1 = vert1;
			Vertex2 = vert2;
		}
	};

	static float degToRad(float degrees) {
		return degrees * (M_PI / 180.0);
	}

	static std::string vec3ToString(glm::vec3 vec) {
		return "[" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + "]";
	}
	

};
#pragma once
#include <vector>
#include <string>
#include <glm/glm/glm.hpp>

class PhysicsUtility {

public: 

	struct Rotation {
		glm::vec3 RotationAxis;
		float Angle; // in degrees

		Rotation() {
			RotationAxis = glm::vec3(0.f);
			Angle = 0.0f;
		}

		Rotation(glm::vec3 axis, float angle) {
			RotationAxis = axis;
			Angle = angle;
		}

		void AddRotation(Rotation rot) {
			RotationAxis += rot.RotationAxis;
		}
	};

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

	static struct Force {
		glm::vec3 Direction;
		float Magnitude;

		Force() {
			Direction = glm::vec3(0.f);
			Magnitude = 0.0f;
		}

		Force(glm::vec3 dir, float mag) {
			Direction = dir;
			Magnitude = mag;
		}
	};

	static struct Torque {
		Rotation rotation;
		float Magnitude;

		Torque() {
			rotation = Rotation();
			Magnitude = 0.0f;
		}

		Torque(Rotation rot, float mag) {
			rotation = rot;
			Magnitude = mag;
		}

		void AddTorque(Rotation rot, float mag) {

		}

		void AddTorque(Torque otherTorque) {

		}
	};

	static std::string vec3ToString(glm::vec3 vec) {
		return "[" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + "]";
	}
	

};
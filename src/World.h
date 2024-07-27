#pragma once
#include <vector>
#include <chrono>
#include <unordered_set>
#include <functional>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Render.h"
#include "MeshLibrary.h"

struct vec3Hash {
	std::size_t operator()(const glm::vec3& v) const {
		// Combine the hash values of x, y, and z components
		std::size_t h1 = std::hash<float>{}(v.x);
		std::size_t h2 = std::hash<float>{}(v.y);
		std::size_t h3 = std::hash<float>{}(v.z);

		// Combine them into a single hash value
		// Use bitwise operations to mix the hash values
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

class World {

protected:
	std::vector<PhysicsObject*> PhysicObjects;
	Renderer renderer;
	glm::vec3 Gravity = glm::vec3(0.0f, -0.98f, 0.0f);

public:

	World();

	GLFWwindow* window;
	Camera camera;

	PhysicsObject* testObj1;
	PhysicsObject* testObj2;

	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();


	void Update();
	void CollisionUpdate();
	void CheckCollision(PhysicsObject& one, PhysicsObject& two, bool& result, glm::vec3& shortestOverlap);
	std::vector<glm::vec3> CalculateCollisionVelocity(PhysicsObject one, PhysicsObject two);
	std::vector<float> ProjectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis);
	void PhysicsUpdate();
	void Render();

	void ProcessInput();
	void ProcessMouseMovement(double xpos, double ypos);
	void ProcessMouseScroll(float yoffset);

	void AddObject(PhysicsObject* object);

	// Collision
	void generateSeparationAxes(std::vector<glm::vec3>& axes, std::vector<glm::vec3>& edges1, std::vector<glm::vec3>& edges2);
	std::pair<float, float> getProjectionRange(const std::vector<Vertex>& vertices, glm::vec3& axis);
	bool intervalsOverlap(const std::pair<float, float>& range1, const std::pair<float, float>& range2);
	bool checkSATCollision(const std::vector<Vertex>& vertices1, const std::vector<Vertex>& vertices2, std::vector<glm::vec3>& axes);

	Renderer GetRenderer() { return renderer; }
	glm::vec3 GetGravity() { return Gravity; }

};
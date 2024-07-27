#pragma once
#include <vector>
#include <glm/glm/glm.hpp>
#include "Render.h"
#include "MeshLibrary.h"

class World {

protected:
	std::vector<PhysicsObject*> PhysicObjects;
	Renderer renderer;
	glm::vec3 Gravity = glm::vec3(0.0f, -0.98f, 0.0f);

public:

	World();

	GLFWwindow* window;
	Camera camera;

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
	float project(const Vertex& vertex, const Vertex& axis);
	std::pair<float, float> getProjectionRange(const std::vector<Vertex>& vertices, const Vertex& axis);
	bool intervalsOverlap(const std::pair<float, float>& range1, const std::pair<float, float>& range2);
	bool checkSATCollision(const std::vector<Vertex>& vertices1, const std::vector<Vertex>& vertices2, const std::vector<Vertex>& axes);

	Renderer GetRenderer() { return renderer; }
	glm::vec3 GetGravity() { return Gravity; }

};
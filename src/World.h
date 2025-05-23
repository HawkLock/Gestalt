#pragma once
#include <vector>
#include <chrono>
#include <unordered_set>
#include <functional>
#include <limits>
#include <algorithm>
#include <filesystem>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/string_cast.hpp> 

#include "Render.h"
#include "MeshLibrary.h"
#include "ConvexShape.h"
#include "GJKAlgorithm.h"
#include "EPAAlgorithm.h"
#include "Raycast.h"

#include "SettingsBus.h"

#include "ScenarioModule.h"

#include "Scenario.h"

#include "GlobalData.h"

#include "Widget.h"


class World {

protected:
	std::vector<PhysicsObject*> PhysicObjects;
	std::vector<TriggerObject*> TriggerObjects;
	Renderer renderer;
	float mu_static = 0.1f;
	float contactThreshold = 0.5;

	PhysicsObject* focusObject;
	bool followFocusedObject = false;

	Scenario scene;

public:

	World();

	GLFWwindow* window;
	Camera camera;
	bool cursorCaptured = true;
	float cursorCooldownTime = 500; // Avoid activating every frame (ms)
	std::chrono::high_resolution_clock::time_point cursorToggleLastTime = std::chrono::high_resolution_clock::now();

	PhysicsObject* testObj1;
	PhysicsObject* testObj2;

	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

	std::vector<Widget*> Widgets;

	void ScanForScenarios();
	void LoadWorld();
	void ScaleWorld();

	void Update();
	void ScenarioUpdate();
	void CollisionUpdate();
	void CheckCollision(PhysicsObject& one, PhysicsObject& two, bool& result, glm::vec3& shortestOverlap);
	std::vector<glm::vec3> CalculateCollisionVelocity(PhysicsObject one, PhysicsObject two);
	std::vector<float> ProjectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis);
	void PhysicsUpdate();
	void Render();
	void CameraUpdate();

	void ProcessInput();
	void ProcessMouseMovement(double xpos, double ypos);
	void ProcessMouseScroll(float yoffset);

	void AddObject(PhysicsObject* object);
	void AddObject(TriggerObject* object);
	void AddWidget(Widget* widget);

	void ClearWorld();

	void LoadScenario(const std::string& filepath, bool clearWorld);

	// Collision
	PhysicsObject* castCameraRay(); // returns closest physics object in the direction the camera is facing
	void resolveOverlap(PhysicsObject* objA, PhysicsObject* objB, const Overlap& overlap);
	void resolveImpulses(PhysicsObject* objA, PhysicsObject* objB, const Overlap& overlap);
	void applyContactForces(PhysicsObject* objA, PhysicsObject* objB, const Overlap& overlap);
	void resolveCollision(PhysicsObject* objA, PhysicsObject* objB, const Overlap& overlap);
	void generateSeparationAxes(std::vector<glm::vec3>& axes, std::vector<glm::vec3>& edges1, std::vector<glm::vec3>& edges2);
	std::pair<float, float> getProjectionRange(const std::vector<Vertex>& vertices, glm::vec3& axis);
	bool intervalsOverlap(const std::pair<float, float>& range1, const std::pair<float, float>& range2);
	float getOverlap(const std::pair<float, float>& range1, const std::pair<float, float>& range2);
	bool checkSATCollision(const std::vector<Vertex>& vertices1, const std::vector<Vertex>& vertices2, std::vector<glm::vec3>& axes, Overlap& smallestOverlap);

	Renderer GetRenderer() { return renderer; }
	std::vector<PhysicsObject*> GetPhysicsObjects() { return PhysicObjects; }
	std::vector<TriggerObject*> GetTriggerObjects() { return TriggerObjects; }
	Scenario GetScene() { return scene; }

	// For User
	
};


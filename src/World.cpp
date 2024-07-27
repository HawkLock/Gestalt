#include "World.h"

World::World()
{
	window = renderer.GetWindow();



	// Define the variables for object1
	glm::vec3 gravity1 = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 position1 = glm::vec3(-0.5f, 0.0f, 0.0f);
	glm::vec3 velocity1 = glm::vec3(0.0f, 0.0f, 0.0f);
	std::vector<glm::vec3> initialActingForces1 = std::vector<glm::vec3>();
	glm::vec3 rotationAxis1 = glm::vec3(0.0f, 0.0f, 0.0f); // glm::vec3(0.5f, 1.0f, 0.7f);
	float angle1 = 50.0f;
	float mass1 = 50.f;
	float faceSize1 = 1.f;

	std::string modelPath1 = "C:/Programming/Gestalt/Models/cube.txt";


	// Create object1 and add it to the world
	PhysicsObject* object1 = new PhysicsObject(position1, velocity1, initialActingForces1, rotationAxis1, angle1, mass1, gravity1, false, faceSize1, modelPath1);
	AddObject(object1);

	// Define the variables for object2
	glm::vec3 gravity2 = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 position2 = glm::vec3(2.0f, 0.0f, 0.f);
	glm::vec3 velocity2 = glm::vec3(0.0f, 0.0f, 0.0f);
	std::vector<glm::vec3> initialActingForces2 = std::vector<glm::vec3>();
	glm::vec3 rotationAxis2 = glm::vec3(0.0f, 0.0f, 0.0f);
	float angle2 = 30.0f;
	float mass2 = 100.0f;
	float faceSize2 = 1.0f;

	std::string modelPath2 = "C:/Programming/Gestalt/Models/cube.txt";

	// Create object2 and add it to the world
	//PhysicsObject* object2 = new PhysicsObject(position2, velocity2, initialActingForces2, rotationAxis2, angle2, mass2, gravity2, false, faceSize2, meshLibrary.getCubeVertices(faceSize2));
	PhysicsObject* object2 = new PhysicsObject(position2, velocity2, initialActingForces2, rotationAxis2, angle2, mass2, gravity2, false, faceSize2, modelPath2);
	AddObject(object2);

	object2->GetMesh().ChangeSize(0.5f);

	std::cout << PhysicObjects.size() << std::endl;
	
}

void World::Update()
{
	// Handles User Input
	ProcessInput();

	// Handles Collision
	CollisionUpdate();

	// Handles Other Physics
	// PhysicsUpdate();

	// Handles Rendering
	Render();
	//std::cout << "Update" << std::endl;
}

void World::CollisionUpdate()
{
	for (int i = 0; i < PhysicObjects.size(); i++)
	{
		for (int z = i+1; z < PhysicObjects.size(); z++)
		{
			//*PhysicObjects[i], * PhysicObjects[z]
			std::vector<float> vertices1 = std::vector<float>();
			std::vector<float> vertices2 = std::vector<float>();

			std::vector<Vertex> obj1Vertices = PhysicObjects[i]->extractVertices(vertices1);
			std::vector<Vertex> obj2Vertices = PhysicObjects[i]->extractVertices(vertices2);

			bool result = false; // checkSATCollision(;
			if (result)
			{
				// Collision reaction
				std::cout << "Collision" << std::endl;
				
			}
		}
	}
}

float World::project(const Vertex& vertex, const Vertex& axis) {
	return vertex.x * axis.x + vertex.y * axis.y + vertex.z * axis.z;
}

std::pair<float, float> World::getProjectionRange(const std::vector<Vertex>& vertices, const Vertex& axis) {
	float minProj = project(vertices[0], axis);
	float maxProj = minProj;

	for (const auto& vertex : vertices) {
		float proj = project(vertex, axis);
		if (proj < minProj) minProj = proj;
		if (proj > maxProj) maxProj = proj;
	}

	return { minProj, maxProj };
}

bool World::intervalsOverlap(const std::pair<float, float>& range1, const std::pair<float, float>& range2) {
	return range1.second >= range2.first && range2.second >= range1.first;
}

bool World::checkSATCollision(const std::vector<Vertex>& vertices1, const std::vector<Vertex>& vertices2, const std::vector<Vertex>& axes) {
	for (const auto& axis : axes) {
		auto range1 = getProjectionRange(vertices1, axis);
		auto range2 = getProjectionRange(vertices2, axis);

		if (!intervalsOverlap(range1, range2)) {
			return false; // Separating axis found, no collision
		}
	}

	return true; // No separating axis found, collision detected
}

// Uses elastic collision calculation
std::vector<glm::vec3> World::CalculateCollisionVelocity(PhysicsObject one, PhysicsObject two) {

	std::vector<glm::vec3> velocities;
	glm::vec3 sumMomentum = one.GetMass() * one.GetCurrentVelocity() + two.GetMass() * two.GetCurrentVelocity();
	float sumMass = one.GetMass() + two.GetMass();
	glm::vec3 initialMomentumOne = one.GetMass() * one.GetCurrentVelocity();
	glm::vec3 velocityOne = (two.GetMass() * (two.GetCurrentVelocity() - one.GetCurrentVelocity()) + sumMomentum) / sumMass;
	glm::vec3 velocityTwo = (one.GetMass() * (one.GetCurrentVelocity() - two.GetCurrentVelocity()) + sumMomentum) / sumMass;
	glm::vec3 finalMomentumOne = one.GetMass() * velocityOne;

	glm::vec3 deltaVelocityOne = velocityOne - one.GetCurrentVelocity();
	glm::vec3 deltaVelocityTwo = velocityTwo - two.GetCurrentVelocity();

	glm::vec3 deltaMomentum = finalMomentumOne - initialMomentumOne;
	glm::vec3 normalForceOne = deltaMomentum;
	glm::vec3 normalForceTwo = normalForceOne * -1.f;

	velocities.push_back(deltaVelocityOne + normalForceOne);
	velocities.push_back(deltaVelocityTwo + normalForceTwo);
	return velocities;
}

// SAT Algorithm Implementation WIP
void World::CheckCollision(PhysicsObject& one, PhysicsObject& two, bool &result, glm::vec3 &shortestOverlap)
{
	std::vector<glm::vec3> oneVertices = one.GetVertices();
	std::vector<glm::vec3> twoVertices = two.GetVertices();

	//std::cout << glm::length(shortestOverlap) << std::endl;
	std::vector<glm::vec3> oneCollisionVertices;
	std::vector<glm::vec3> twoCollisionVertices;

	for (unsigned int i = 0; i < oneVertices.size(); i++)
	{
		glm::vec3 vA = oneVertices[i];
		glm::vec3 vB = oneVertices[(i + 1) % oneVertices.size()];

		glm::vec3 edge = vB - vA;
		// the normal of the edge
		glm::vec3 axis(-edge.y, edge.x, edge.z);


		// Goes {min, max}
		std::vector<float> extentOneValues = ProjectVertices(oneVertices, axis);
		std::vector<float> extentTwoValues = ProjectVertices(twoVertices, axis);

		if (extentOneValues[0] > extentTwoValues[1] || extentTwoValues[0] > extentOneValues[1]) {
			result = false;
			return;
		}

		/*
		float overlapOne = extentOneValues[0] - extentTwoValues[1];
		float overlapTwo = extentTwoValues[0] - extentOneValues[1];
		*/
		float overlapOne = extentOneValues[0] - extentTwoValues[0];
		float overlapTwo = extentTwoValues[1] - extentOneValues[1];
		// Determines which is the overlapping part
		if (extentOneValues[0] - extentTwoValues[1] < 0) {
			// Min of #1 is less than max of #2
			if (glm::length(shortestOverlap) > overlapOne) {
				shortestOverlap = axis * overlapOne;
			}
		}
		else {
			// Min of #2 is less than max of #1
			if (glm::length(shortestOverlap) > overlapTwo) {
				shortestOverlap = axis * overlapTwo;
			}
		}
		//printf("Vertex: %f, %f, %f \n", oneVertices[i].x, oneVertices[i].y, oneVertices[i].z);
		oneCollisionVertices.push_back(oneVertices[i]);
	}

	for (unsigned int i = 0; i < twoVertices.size(); i++)
	{
		glm::vec3 vA = twoVertices[i];
		glm::vec3 vB = twoVertices[(i + 1) % twoVertices.size()];

		glm::vec3 edge = vB - vA;
		// the normal of the edge
		glm::vec3 axis(-edge.y, edge.x, edge.z);


		// Goes {min, max}
		std::vector<float> extentOneValues = ProjectVertices(oneVertices, axis);
		std::vector<float> extentTwoValues = ProjectVertices(twoVertices, axis);

		if (extentOneValues[0] > extentTwoValues[1] || extentTwoValues[0] > extentOneValues[1]) {
			result = false;
			return;
		}

		float overlapOne = extentTwoValues[1] - extentOneValues[0];
		float overlapTwo = extentOneValues[1] - extentTwoValues[0];

		// Determines which is the overlapping part
		if (extentOneValues[0] - extentTwoValues[1] < 0) {
			// Min of #1 is less than max of #2
			if (glm::length(shortestOverlap) > overlapOne) {
				shortestOverlap = axis * overlapOne;
			}
		}
		else {
			// Min of #2 is less than max of #1
			if (glm::length(shortestOverlap) > overlapTwo) {
				shortestOverlap = axis * overlapTwo;
			}
		}
		twoCollisionVertices.push_back(twoVertices[i]);
	}

	std::vector<PhysicsUtility::Edge> oneCollisionEdges;
	std::vector<PhysicsUtility::Edge> twoCollisionEdges;
	for (int i = 1; i < oneCollisionVertices.size(); i+=2) {
		oneCollisionEdges.push_back(PhysicsUtility::Edge(oneCollisionVertices[i-1], oneCollisionVertices[i]));
	}
	for (int i = 1; i < twoCollisionVertices.size(); i+=2) {
		twoCollisionEdges.push_back(PhysicsUtility::Edge(twoCollisionVertices[i-1], twoCollisionVertices[i]));
	}
	/*
	printf("One Vertex Count: %d\n", oneCollisionVertices.size());
	printf("One Edge Count: %d\n", oneCollisionEdges.size());
	printf("Two Vertex Count: %d\n", twoCollisionVertices.size());
	printf("Two Vertex Count: %d\n", twoCollisionEdges.size());
	*/

	//std::cout << "ShortestOverlap: " << glm::length(shortestOverlap) << std::endl;

	// If there are no gaps between the objects
	result = true;
}

std::vector<float> World::ProjectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis)
{
	float min = FLT_MAX;
	float max = -min;

	for (int i = 0; i < vertices.size(); i++) 
	{
		glm::vec3 v = vertices[i];
		float proj = glm::dot(v, axis);

		if (proj < min) {
			min = proj;
		}
		if (proj > max) {
			max = proj;
		}
	}

	return { min, max };
}

void World::PhysicsUpdate()
{
	// Handles Physics
	for (int i = 0; i < PhysicObjects.size(); i++)
	{
		if (!PhysicObjects[i]->IsAnchored()) {
			//std::cout << "Object " << i << " velocity: " << PhysicsUtility::vec3ToString(PhysicObjects[i].GetCurrentVelocity()) << " units/unit" << std::endl;
			PhysicObjects[i]->CalculatePhysics(renderer.GetDeltaTime());
		}
	}
}

void World::Render()
{
	renderer.RenderLoop(&camera, PhysicObjects);
}

void World::AddObject(PhysicsObject* object)
{
	PhysicObjects.push_back(object);
}

void World::ProcessInput()
{
	float cameraSpeed = 7.5f * renderer.deltaTime;

	// WASD Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.cameraPos += cameraSpeed * camera.cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.cameraPos -= cameraSpeed * camera.cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
	}

	// Altitude Movement
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.cameraPos -= cameraSpeed * glm::vec3(0, 1, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.cameraPos += cameraSpeed * glm::vec3(0, 1, 0);
	}

	// Program Controls
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Debuging Controls

}

void World::ProcessMouseMovement(double xpos, double ypos)
{
	camera.ProcessMouseMovement(xpos, ypos);
}

void World::ProcessMouseScroll(float yoffset)
{
	camera.ProcessMouseScroll(yoffset, renderer.deltaTime);
}
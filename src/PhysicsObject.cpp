#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(glm::vec3 initialPosition, glm::vec3 initialVelocity, glm::vec3 initialNetForce, glm::vec3 rotationAxis, float angle, float initialMass, glm::vec3 gravity, bool isAnchored, float faceSize)
{
	pos = initialPosition;
	velocity = initialVelocity;
	NetForce = initialNetForce;
	Mass = initialMass;
	PhysicsUtility::Rotation tempRotation; tempRotation.RotationAxis = rotationAxis; tempRotation.Angle = angle;
	rotation = tempRotation;
	Anchored = isAnchored;

	AddForce(gravity, "Gravity");

	Model = Mesh(initialPosition, faceSize);
}

PhysicsObject::PhysicsObject(glm::vec3 initialPosition, glm::vec3 initialVelocity, std::vector<glm::vec3> initialActingForcesVectors, glm::vec3 rotationAxis, float angle, float initialMass, glm::vec3 gravity, bool isAnchored, float faceSize) {
	pos = initialPosition;
	velocity = initialVelocity;
	generateForceVectorFromVec3Vector(initialActingForcesVectors);
	Mass = initialMass;
	PhysicsUtility::Rotation tempRotation; tempRotation.RotationAxis = rotationAxis; tempRotation.Angle = angle;
	rotation = tempRotation;
	Anchored = isAnchored;

	AddForce(gravity, "Gravity");

	Model = Mesh(initialPosition, faceSize);
}


// Force Manipulation Functions
unsigned int PhysicsObject::FindForceIndex(std::string forceName)
{
	for (unsigned int i = 0; i < ActingForces.size(); i++)
	{
		if (ActingForces[i].ForceName == forceName)
		{
			return i;
		}
	}
	return -1;
	// If the given force does not exist in the acting forces vector of the physics object
}
void PhysicsObject::AddForce(glm::vec3 newForceVector, std::string forceName)
{
	Force newForce;
	newForce.ForceVector = newForceVector;
	newForce.ForceName = forceName;
	ActingForces.push_back(newForce);
	CalculateNetForce();
}
void PhysicsObject::RemoveForce(std::string forceName)
{
	unsigned int forceIndex = FindForceIndex(forceName);
	// If there is no force by that name
	if (forceIndex == -1) {
		return;
	}
	ActingForces.erase(ActingForces.begin() + forceIndex - 1);
	CalculateNetForce();
}

void PhysicsObject::AddInstantaneousForce(glm::vec3 instantForce) {
	//pos += instantForce;
	Force newForce;
	newForce.ForceName = "InstantForce";
	newForce.ForceVector = instantForce;
	ForceStack.push(newForce);
}

void PhysicsObject::AddInstantaneousTorque(PhysicsUtility::Torque torque) {
	TorqueStack.push(torque);
}

void PhysicsObject::CalculateNetForce()
{
	glm::vec3 newNetForce = glm::vec3(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < ActingForces.size(); i++)
	{
		newNetForce += ActingForces[i].ForceVector;
	}
	while (!ForceStack.empty()) {
		newNetForce += ForceStack.top().ForceVector;
		ForceStack.pop();
	}
	NetForce = newNetForce;
	glm::vec3 acceleration = NetForce / Mass;
	velocity += acceleration;
}

void PhysicsObject::CalculateNetTorque() {
	PhysicsUtility::Torque newNetTorque = PhysicsUtility::Torque();
	for (unsigned int i = 0; i < ActingTorques.size(); i++) {
		newNetTorque.AddTorque(ActingTorques[i]);
	}
	while (!TorqueStack.empty()) {
		newNetTorque.AddTorque(TorqueStack.top());
		TorqueStack.pop();
	}
	NetTorque = newNetTorque;
}

void PhysicsObject::CalculatePhysics(float deltaTime)
{
	CalculateNetForce();
	pos += velocity * deltaTime;
	//applyFriction(deltaTime);
	//printf("Pos: %f, %f, %f \n", pos.x, pos.y, pos.z);
}

std::vector<Force> PhysicsObject::generateForceVectorFromVec3Vector(std::vector<glm::vec3> vec3Vector) {
	std::vector<Force> forceVector = std::vector<Force>();
	for (glm::vec3 vec : vec3Vector) {
		AddForce(vec, "");
	}
	return forceVector;
}

void PhysicsObject::applyFriction(float modifier)
{
	velocity -= velocity * (universalVelocityDecay * modifier);
}

std::vector<glm::vec3> PhysicsObject::GetVertices()
{
	// Retrieves all the corner vertices from the mesh
	std::vector<glm::vec3> vertices;
	for (int i = 0; i < sizeof(Model.vertices) / sizeof(Model.vertices[0]);)
	{
		glm::vec3 vertex = glm::vec3(Model.vertices[i], Model.vertices[i + 1], Model.vertices[i + 2]) + pos;
		vertices.push_back(vertex);
		i += 5;
	}
	// Gets indices of all duplicate vertices
	std::vector<float> duplicateVertexIndices;
	for (int i = 0; i < vertices.size(); i++)
	{
		for (int z = i + 1; z < vertices.size(); z++)
		{
			if (!std::count(duplicateVertexIndices.begin(), duplicateVertexIndices.end(), z))
			{
				if (vertices[i] == vertices[z])
				{
					duplicateVertexIndices.push_back(z);
				}
			}
		}
	}
	// Removes duplicate vertices
	for (int i = 0; i < duplicateVertexIndices.size(); i++)
	{
		vertices[duplicateVertexIndices[i]] = vertices.back();
	}
	for (int i = 0; i < duplicateVertexIndices.size(); i++)
	{
		vertices.pop_back();
	}

	return vertices;
}
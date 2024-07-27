#pragma once
#include <glm/glm/glm.hpp>
#include <iostream>
#include <vector>
#include <stack>

#include "PhysicsUtility.h"
#include "Mesh.h"

// The datatype for a force
struct Force {
	glm::vec3 ForceVector;
	std::string ForceName;
};

class PhysicsObject {

protected:
	float universalVelocityDecay = 0.5; 

	glm::vec3 pos;
	glm::vec3 velocity; // In m/s
	glm::vec3 NetForce; // In N
	
	PhysicsUtility::Torque NetTorque; // In N
	PhysicsUtility::Rotation rotation;
	std::vector<Force> ActingForces; // A set of all the currently acting forces on the physics object
	std::vector<PhysicsUtility::Torque> ActingTorques;
	float Mass; // In Kg
	bool Anchored;
	std::stack<Force> ForceStack;
	std::stack<PhysicsUtility::Torque> TorqueStack;

	Mesh Model;

public:

	PhysicsObject(glm::vec3 initialPosition, glm::vec3 initialVelocity, std::vector<glm::vec3> initialActingForcesVectors, glm::vec3 rotationAxis, float angle, float initialMass, glm::vec3 gravity, bool isAnchored, float faceSize, std::vector<float> inputVertices);
	PhysicsObject(glm::vec3 initialPosition, glm::vec3 initialVelocity, std::vector<glm::vec3> initialActingForcesVectors, glm::vec3 rotationAxis, float angle, float initialMass, glm::vec3 gravity, bool isAnchored, float faceSize, std::string& modelPath);

	// Getters and Setters
	glm::vec3 GetCurrentPos() { return pos; }
	glm::vec3 GetCurrentVelocity() { return velocity; }
	glm::vec3 GetCurrentNetForce() { return NetForce; }
	glm::vec3 GetRotationAxis() { return rotation.RotationAxis; }
	float GetRotationAngle() { return rotation.Angle; }
	float GetMass() { return Mass; }
	Mesh& GetMesh() { return Model; }
	float GetFaceExtent() { return Model.faceLength; }
	float IsAnchored() { return Anchored; }

	void SetPos(glm::vec3 newPos) { pos = newPos; }
	void SetVelocity(glm::vec3 newVelocity) { velocity = newVelocity; }
	void SetMass(float newMass) { Mass = newMass; }
	void SetAngle(glm::vec3 rotationAxis, float angle) { rotation.RotationAxis = rotationAxis;  rotation.Angle = angle; }

	// These forces are used for all forces currently acting on the physics objects
	unsigned int FindForceIndex(std::string forceName);
	void AddForce(glm::vec3 newForceVector, std::string forceName);
	void RemoveForce(std::string forceName);
	void CalculateNetForce();
	void CalculateNetTorque();
	void AddInstantaneousForce(glm::vec3 instantForce); // Used for forces that go away immediately
	void AddInstantaneousTorque(PhysicsUtility::Torque torque); // Used for torques that go away immediately

	// Collision
	std::vector<Vertex> extractVertices(const std::vector<float>& data);

	void CalculatePhysics(float deltaTime);

	std::vector<glm::vec3> GetVertices();
	int getForceStackDepth() {
		return ForceStack.size();
	}

	void RenderMesh(const Shader& shader, const glm::mat4& modelMatrix, GLuint textureID); 

private: 
	std::vector<Force> generateForceVectorFromVec3Vector(std::vector<glm::vec3> vec3Vector);
	void applyFriction(float modifier); // Decays the velocity to add the idea of friction

};
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

	//glm::vec3 gravityVec = glm::vec3(0, -9.8, 0);
	glm::vec3 gravityVec = glm::vec3(0.0f, 0.0f, 0.0f); // disabled gravity

	glm::vec3 pos;
	glm::quat rot;
	float sideLength = 1;

	glm::vec3 acceleration;
	glm::vec3 velocity; // In m/s
	glm::vec3 angularAcceleration;
	glm::vec3 angularVelocity;

	
	float Mass; // In Kg
	glm::mat3 inertiaTensor;
	glm::mat3 inverseInertiaTensor;
	float momentOfInertia;


	bool Anchored;
	std::vector<Force> continuousForces;

	Mesh Model;

public:

	PhysicsObject(glm::vec3 initialPosition, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, bool isAnchored, float faceSize, std::string& modelPath);

	// Getters and Setters
	glm::vec3 GetCurrentPos() { return pos; }
	glm::quat GetCurrentRot() { return rot; }

	glm::vec3 GetCurrentVelocity() { return velocity; }
	glm::vec3 GetCurrentAngularVelocity() { return angularVelocity; }
	glm::vec3 GetCurrentNetForce() { return glm::vec3(); }

	float GetMass() { return Mass; }
	float GetMomentOfInertia() { return momentOfInertia; }
	glm::mat3 GetInverseInertiaTensor() { return inverseInertiaTensor; }
	Mesh& GetMesh() { return Model; }
	float GetFaceExtent() { return Model.faceLength; }
	float IsAnchored() { return Anchored; }

	void SetPos(glm::vec3 newPos) { pos = newPos; }
	void SetRot(glm::quat newRot) { rot = newRot; }

	void SetVelocity(glm::vec3 newVelocity) { velocity = newVelocity; }
	void SetAngularVelocity(glm::vec3 newVelocity) { angularVelocity = newVelocity; }
	void SetMass(float newMass) { Mass = newMass; }

	void AddRotation(glm::quat addRot) { rot = glm::normalize(addRot * rot); }

	// Collision
	std::vector<Vertex> extractVertices(const std::vector<float>& data);

	// Physics
	void AddForce(Force newForce, bool isContinuous);
	void CalculateAcceleration();
	void CalculateVelocity(float deltaTime);
	void CalculatePosition(float deltaTime);
	void CalculatePhysics(float deltaTime);

	float CalculateTranslationalEnergy();
	float CalculateRotationalEnergy();
	float CalculateTotalEnergy();

	std::vector<glm::vec3> GetVertices();

	void RenderMesh(const Shader& shader, GLuint textureID); 

private: 
	void applyFriction(float modifier); // Decays the velocity to add the idea of friction

};
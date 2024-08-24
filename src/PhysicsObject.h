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

	 glm::vec3 gravityVec = glm::vec3(0, -9.8, 0);
	// glm::vec3 gravityVec = glm::vec3(0, 0.0, 0); // disabled

	glm::vec3 pos;
	glm::quat rot;

	glm::vec3 acceleration;
	glm::vec3 velocity; // In m/s
	
	float Mass; // In Kg
	bool Anchored;
	std::vector<Force> continuousForces;

	Mesh Model;

public:

	PhysicsObject(glm::vec3 initialPosition, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, bool isAnchored, float faceSize, std::string& modelPath);

	// Getters and Setters
	glm::vec3 GetCurrentPos() { return pos; }
	glm::quat GetCurrentRot() { return rot; }

	glm::vec3 GetCurrentVelocity() { return velocity; }
	glm::vec3 GetCurrentNetForce() { return glm::vec3(); }

	float GetMass() { return Mass; }
	Mesh& GetMesh() { return Model; }
	float GetFaceExtent() { return Model.faceLength; }
	float IsAnchored() { return Anchored; }

	void SetPos(glm::vec3 newPos) { pos = newPos; }
	void SetRot(glm::quat newRot) { rot = newRot; }

	void SetVelocity(glm::vec3 newVelocity) { velocity = newVelocity; }
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

	std::vector<glm::vec3> GetVertices();

	void RenderMesh(const Shader& shader, GLuint textureID); 

private: 
	void applyFriction(float modifier); // Decays the velocity to add the idea of friction

};
#pragma once
#include <glm/glm/glm.hpp>
#include <iostream>
#include <vector>
#include <stack>

#include "Mesh.h"

class TriggerObject {

protected:

	float sideLength = 1;

	bool Anchored;

	Mesh Model;

public:
	glm::vec3 pos;
	glm::quat rot;

	glm::vec3 acceleration;
	glm::vec3 velocity; // In m/s
	glm::vec3 angularAcceleration;
	glm::vec3 angularVelocity;

	PhysicsObject(glm::vec3 initialPosition, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, bool isAnchored, float faceSize, std::string& modelPath);

	// Getters and Setters
	glm::vec3 GetCurrentPos() { return pos; }
	glm::quat GetCurrentRot() { return rot; }

	glm::vec3 GetCurrentVelocity() { return velocity; }
	glm::vec3 GetCurrentAngularVelocity() { return angularVelocity; }
	glm::vec3 GetCurrentNetForce() { return glm::vec3(); }

	float GetMass() { return Mass; }
	float GetMomentOfInertia() { return momentOfInertia; }

	glm::mat3 GetInertiaTensor() {
		glm::mat3 rotationMatrix = glm::mat3_cast(rot);
		return rotationMatrix * inertiaTensor * glm::transpose(rotationMatrix);
	}

	glm::mat3 GetInverseInertiaTensor() {
		glm::mat3 rotationMatrix = glm::mat3_cast(rot);
		glm::mat3 worldInertiaTensor = rotationMatrix * inertiaTensor * glm::transpose(rotationMatrix);
		return glm::inverse(inertiaTensor);
	}

	Mesh& GetMesh() { return Model; }
	float GetFaceExtent() { return Model.faceLength; }
	float IsAnchored() { return Anchored; }
	float GetRestitution() { return restitution; }
	float GetStaticFriction() { return staticFriction; }
	float GetKineticFriction() { return kineticFriction; }

	void SetPos(glm::vec3 newPos) { pos = newPos; }
	void SetRot(glm::quat newRot) { rot = newRot; }

	void SetVelocity(glm::vec3 newVelocity) { velocity = newVelocity; }
	void SetAngularVelocity(glm::vec3 newVelocity) { angularVelocity = newVelocity; }
	void SetMass(float newMass) { Mass = newMass; }

	void AddRotation(glm::quat addRot) { rot = glm::normalize(addRot * rot); }

	void ScaleSize(float scale);

	// Collision
	std::vector<Vertex> extractVertices(const std::vector<float>& data);

	// Physics
	glm::vec3 CalculateCOM();
	void CalculateInertiaTensor(glm::mat3& tensor);

	void AddForce(Force newForce, bool isContinuous);
	void CalculateAcceleration();
	void CalculateVelocity(float deltaTime);
	void CalculatePosition(float deltaTime);
	void CalculatePhysics(float deltaTime);

	float CalculateTranslationalEnergy();
	float CalculateRotationalEnergy();
	float CalculateTotalEnergy();

	void ApplyForce(const glm::vec3& force, const glm::vec3& contactPoint);

	std::vector<glm::vec3> GetVertices();

	void RenderMesh(const Shader& shader, GLuint textureID);

private:
	void applyFriction(float modifier); // Decays the velocity to add the idea of friction

};
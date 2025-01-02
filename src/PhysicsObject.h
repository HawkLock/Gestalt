#pragma once
#include <glm/glm/glm.hpp>
#include <iostream>
#include <vector>
#include <stack>

#include "RenderUtils.h"
#include "PhysicsUtility.h"
#include "Mesh.h"

#include <glm/glm/gtx/quaternion.hpp>

// The datatype for a force
struct Force {
	glm::vec3 ForceVector;
	std::string ForceName;
};

class PhysicsObject {
public: 
	struct AABB {
		glm::vec3 min;  // Minimum corner of the AABB (world-space coordinates)
		glm::vec3 max;  // Maximum corner of the AABB (world-space coordinates)

		// Constructor to initialize the AABB with min and max values
		AABB(const glm::vec3& min = glm::vec3(0.0f), const glm::vec3& max = glm::vec3(0.0f))
			: min(min), max(max) {}

		// Function to check if a point is inside the AABB
		bool contains(const glm::vec3& point) const {
			return point.x >= min.x && point.x <= max.x &&
				point.y >= min.y && point.y <= max.y &&
				point.z >= min.z && point.z <= max.z;
		}

		// Function to check if two AABBs overlap
		bool intersects(const AABB& other) const {
			return !(max.x < other.min.x || min.x > other.max.x ||
				max.y < other.min.y || min.y > other.max.y ||
				max.z < other.min.z || min.z > other.max.z);
		}
	};

protected:

	//glm::vec3 gravityVec = glm::vec3(0, -9.8, 0);
	glm::vec3 gravityVec = glm::vec3(0.0f, 0.0f, 0.0f); // disabled gravity

	float sideLength = 1;
	
	float Mass; // In Kg
	glm::mat3 inertiaTensor;
	glm::mat3 inverseInertiaTensor;
	float momentOfInertia;

	float restitution = 0.2f;
	float staticFriction = 0.4f;
	float kineticFriction = 0.2;

	bool Anchored;
	std::vector<Force> continuousForces;

	Mesh Model;
	Mesh ArrowModel;

	float minimumArrowLength = 1; // It can be zero, but if it is anything above zero, it should still poke out of the model
	float arrowModelOffset;

public:
	glm::vec3 pos;
	glm::quat rot;

	glm::vec3 acceleration;
	glm::vec3 velocity; // In m/s
	glm::vec3 angularAcceleration;
	glm::vec3 angularVelocity;

	PhysicsObject(glm::vec3 initialPosition, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, bool isAnchored, float faceSize, std::string& modelPath, std::string& arrowModelPath);

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
	Mesh* GetMeshPtr() { return &Model; }
	float GetFaceExtent() { return Model.faceLength; }
	float IsAnchored() { return Anchored; }
	float GetRestitution() { return restitution; }
	float GetStaticFriction() { return staticFriction; }
	float GetKineticFriction() { return kineticFriction; }
	glm::vec3 CalculateCenter();
	float CalculateRadius(const glm::vec3 center);
	void CalculateBoundingSphere(glm::vec3& center, float& radius);
	AABB GetWorldAABB();

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

	void ScaleArrowModel(float size);

	void RenderMesh(const Shader& shader, GLuint textureID); 
	void RenderArrows(const Shader& shader, GLuint velocityTextureID, GLuint accelerationTextureID);

private: 
	void applyFriction(float modifier); // Decays the velocity to add the idea of friction

};
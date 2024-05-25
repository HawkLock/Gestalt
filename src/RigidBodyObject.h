#pragma once
#include "PhysicsObject.h"

// Currently its only a cube
class RigidBodyObject : public PhysicsObject {

protected:

public: 
	RigidBodyObject(glm::vec3 initialPosition, glm::vec3 initialVelocity, glm::vec3 initialNetForce, glm::vec3 rotationAxis, float angle, float initialMass, glm::vec3 gravity, bool isAnchored, float faceSize);

	void SetPos(glm::vec3 newPos);

};
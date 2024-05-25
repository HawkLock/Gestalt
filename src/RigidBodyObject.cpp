#include "RigidBodyObject.h"

RigidBodyObject::RigidBodyObject(glm::vec3 initialPosition, glm::vec3 initialVelocity, glm::vec3 initialNetForce, glm::vec3 rotationAxis, float angle, float initialMass, glm::vec3 gravity, bool isAnchored, float faceSize) : PhysicsObject(initialPosition, initialVelocity, initialNetForce, rotationAxis, angle, initialMass, gravity, isAnchored, faceSize)
{
	
}

void RigidBodyObject::SetPos(glm::vec3 newPos)
{
	// Moves all child objects of the object as well (mainly the mesh)
	Model.Move(newPos);
	pos = newPos;
}
#pragma once
#include "PhysicsObject.h"

class PObj {
	PhysicsObject* obj;
public:

	PObj(PhysicsObject* p) { obj = p; }

	PhysicsObject* GetObject() { return obj; }

	glm::vec3 GetPosition() const {
		return obj->pos;
	}
	glm::quat GetRotation() const {
		return obj->rot;
	}
	glm::vec3 GetVelocity() const {
		return obj->velocity;
	}
	glm::vec3 GetAcceleration() const {
		return obj->acceleration;
	}
	glm::vec3 GetAngularVelocity() const {
		return obj->angularVelocity;
	}
	glm::vec3 GetAngularAcceleration() const {
		return obj->angularAcceleration;
	}
	float GetMass() const {
		return obj->GetMass();
	}
	void SetPosition(glm::vec3 pos) {
		obj->pos = pos;
	}
	void SetRotation(glm::quat rot) {
		obj->rot = rot;
	}
	void SetVelocity(glm::vec3 vel) {
		obj->velocity = vel;
	}
	void SetAcceleration(glm::vec3 acel) {
		obj->acceleration = acel;
	}
	void SetAngularVelocity(glm::vec3 vel) {
		obj->angularVelocity = vel;
	}
	void SetAngularAcceleration(glm::vec3 acel) {
		obj->angularAcceleration = acel;
	}
	void SetMass(float mass) {
		obj->SetMass(mass);
	}

	void Scale(float scale) {
		obj->ScaleSize(scale);
		obj->scale = scale;
	}

};
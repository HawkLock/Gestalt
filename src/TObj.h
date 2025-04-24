#pragma once
#include "TriggerObject.h"

class TObj {
	TriggerObject* obj;
public:

	TObj(TriggerObject* t) { obj = t; }

	TriggerObject* GetObject() { return obj; }

	glm::vec3 GetPosition() const {
		return obj->pos;
	}
	glm::quat GetRotation() const {
		return obj->rot;
	}

	void SetPosition(glm::vec3 pos) {
		obj->pos = pos;
	}
	void SetRotation(glm::quat rot) {
		obj->rot = rot;
	}
	void Scale(float scale) {
		obj->ScaleSize(scale);
		obj->scale = scale;
	}

	void setTrigger(void (*func)(std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point)) { obj->triggerPtr = func; }

};
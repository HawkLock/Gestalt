#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "PhysicsObject.h"
#include "TriggerObject.h"

class Scenario {

public:
	std::string name;
	std::vector<PhysicsObject*> physicsObjects;
	std::vector<TriggerObject*> triggerObjects;
	glm::vec3 gravity;

	void LoadScenarioFromFile(const std::string& filepath);

	static void SaveScenarioToFile(const std::string& filepath, const std::string& name, std::vector<PhysicsObject*> PhysicObjects, std::vector<TriggerObject*> TriggerObjects, glm::vec3 gravity);
};
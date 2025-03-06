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

#include "GlobalData.h"

#include "PhysicsObject.h"
#include "TriggerObject.h"


#include "BaseLessonModule.h"

#include "EnergyLessonModule.h"
#include "ProjectileMotionLessonModule.h"

class Scenario {
private:

public:
	std::string name;
	std::vector<PhysicsObject*> physicsObjects;
	std::vector<TriggerObject*> triggerObjects;

	glm::vec3 gravity;
	float scale;
	bool lockedRotation;
	float timeScale;

	std::string title;
	std::string lessonPath;
	std::string explanation;
	std::string scenario;
	std::string lessonModuleID;
	BaseLessonModule* lessonModule;

	void LoadScenarioFromFile(const std::string& filepath);
	void LoadLessonFromFile(const std::string& filepath);
	
	void LoadLessonModule(std::string id);

	void AssignScenarioSettings();

	static void SaveScenarioToFile(const std::string& filepath, const std::string& name, std::vector<PhysicsObject*> PhysicObjects, std::vector<TriggerObject*> TriggerObjects, Scenario scene);
};
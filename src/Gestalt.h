#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "World.h"
#include "PhysicsUtility.h"
#include "PObj.h"
#include "TObj.h"


enum Shape {
	CUBE,
	SPHERE,
	PLANE,
	PYRAMID,
	SLIVER,
	ARROW
};

class Gestalt {
	GLFWwindow* window;

	// Default Texture Paths 
	std::string texturePath1 = "../Textures/CrateTexture.jpg";
	std::string texturePath2 = "../Textures/Blue.png";
	// Default Object Paths
	std::string arrowModelPath = "../Models/arrow.txt";
	std::string cubePath = "../Models/cube1.txt";
	std::string spherePath = "../Models/ico.txt";
	std::string planePath = "../Models/plane.txt";
	std::string pyramidPath = "../Models/pyramid.txt";
	std::string sliverPath = "../Models/sliver.txt";

	// Default values
	glm::vec3 defaultPosition = glm::vec3();
	glm::quat defaultRotation = glm::quat();
	float defaultMass = 2;
	float defaultSize = 2;

public:
	static World* world;

	Gestalt() { world = new World(); }

	void Initialize();

	void Run();

	PObj* CreateObject(Shape shape, float size, bool anchored) {
		std::string modelPath;
		switch (shape) {
		case CUBE:
			modelPath = cubePath;
			break;
		case SPHERE:
			modelPath = spherePath;
			break;
		case PLANE:
			modelPath = planePath;
			break;
		case PYRAMID:
			modelPath = pyramidPath;
			break;
		case SLIVER:
			modelPath = sliverPath;
			break;
		case ARROW:
			modelPath = arrowModelPath;
			break;
		default:
			break;
		}
		std::cout << modelPath << std::endl;
		PhysicsObject* obj = new PhysicsObject(defaultPosition, std::vector<glm::vec3>(), defaultRotation, defaultMass, anchored, defaultSize, modelPath, texturePath1, arrowModelPath);
		obj->scale = size;
		obj->ScaleSize(size);
		PObj* pObj = new PObj(obj);
		return pObj;
	}

	TObj* CreateObject(Shape shape, float size) {
		std::string modelPath;
		switch (shape) {
		case CUBE:
			modelPath = cubePath;
			break;
		case PLANE:
			modelPath = planePath;
			break;
		case PYRAMID:
			modelPath = pyramidPath;
			break;
		case SLIVER:
			modelPath = sliverPath;
			break;
		case ARROW:
			modelPath = arrowModelPath;
			break;
		default:
			break;
		}
		std::cout << modelPath << std::endl;
		TriggerObject* obj = new TriggerObject(defaultPosition, defaultRotation, defaultSize, modelPath, texturePath2);
		obj->scale = size;
		obj->ScaleSize(size);
		TObj* tObj = new TObj(obj);
		return tObj;
	}

	void AddObjectToScene(PObj* pObj) {
		world->AddObject(pObj->GetObject());
	}

	void AddObjectToScene(TObj* tObj) {
		world->AddObject(tObj->GetObject());
	}

	void SaveScenario(std::string name) {
		std::string path = "../Scenarios/" + name + ".txt";
		Scenario::SaveScenarioToFile(path, name, world->GetPhysicsObjects(), world->GetTriggerObjects(), world->GetScene());
	}

};


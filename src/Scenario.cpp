#pragma once
#include "Scenario.h"

/*
Scene File Format:
// Scenario Data: sData 
            Name (std::string)
            Gravity (glm::vec3)
            scale (float)
            locked rotation (bool)

// Lesson Data: lData
            Title (std::string)
            Lesson Text Path (std::string)

//  Settings
            Gravity (glm::vec3)

//  PHYSICS OBJECT: "pObj" (one line): 
            Position (glm::vec3)
            Rotation (glm::quat)
            Velocity (glm::vec3)
            Acceleration (glm::vec3)
            Mass (float)
            Anchored? (bool)
            Face Size (float)
            Model Path (std::string)
            Texture Path (std::string)
            Arrow Path (std::string)
            numberOfForce (int)
            Acting Forces (std::vector<glm::vec3>)

//  TRIGGER OBJECT: "tObj" (one line): 
            Position (glm::vec3)
            Rotation (glm::quat)
            Face Size (float)
            Model Path (std::string)
            Texture Path (std::string)

Lesson File Format:
            Title (std::string)
            Lesson (std::string)
*/

void errorMessage(std::string line, std::string specifier) {
    std::cerr << "Error parsing line for \"" << specifier << "\": " << line << std::endl;
}

void Scenario::LoadScenarioFromFile(const std::string& filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "sData") {
            int lockedRotInt;

            if (!(iss >> name)) {
                errorMessage(line, "Scenario name parsing failed");
            }
            if (!(iss >> gravity.x >> gravity.y >> gravity.z)) {
                errorMessage(line, "Gravity (x, y, z) parsing failed");
            }
            if (!(iss >> scale)) {
                errorMessage(line, "Scale parsing failed");
            }
            if (!(iss >> lockedRotInt)) {
                errorMessage(line, "Locked Rotation Bool parsing failed");
            }
            if (!(iss >> timeScale)) {
                errorMessage(line, "Time scale parsing failed");
            }
            lockedRotation = static_cast<bool>(lockedRotInt);

            AssignScenarioSettings();
        }

        if (type == "lData") {
            if (!(iss >> title)) {
                errorMessage(line, "Lesson title parsing failed");
            }
            if (!(iss >> lessonPath)) {
                errorMessage(line, "Lesson path parsing failed");
            }
            if (!(iss >> lessonModuleID)) {
                errorMessage(line, "Lesson module ID parsing failed");
            }
        }

        if (type == "pObj") {
            // Read edge data

            glm::vec3 pos;
            glm::quat rot;
            glm::vec3 vel;
            glm::vec3 acc;
            float mass;
            int anchoredInt;
            bool anchored;
            float faceSize;
            float objScale;
            std::string modelPath;
            std::string texturePath;
            std::string arrowPath;
            std::vector<glm::vec3> forces;

            int numberOfForces;

            if (!(iss >> pos.x >> pos.y >> pos.z)) {
                errorMessage(line, "Position (pos.x, pos.y, pos.z) parsing failed");
            }
            if (!(iss >> rot.w >> rot.x >> rot.y >> rot.z)) {
                errorMessage(line, "Rotation (rot.w, rot.x, rot.y, rot.z) parsing failed");
            }
            if (!(iss >> vel.x >> vel.y >> vel.z)) {
                errorMessage(line, "Velocity (vel.x, vel.y, vel.z) parsing failed");
            }
            if (!(iss >> acc.x >> acc.y >> acc.z)) {
                errorMessage(line, "Acceleration (acc.x, acc.y, acc.z) parsing failed");
            }
            if (!(iss >> mass)) {
                errorMessage(line, "Mass parsing failed");
            }
            if (!(iss >> anchoredInt >> faceSize)) {
                errorMessage(line, "Anchored state (anchoredInt) or faceSize parsing failed");
            }
            if (!(iss >> objScale)) {
                errorMessage(line, "Physics Object scale parsing failed");
            }
            if (!(iss >> modelPath >> texturePath >> arrowPath)) {
                errorMessage(line, "File paths (modelPath, texturePath, arrowPath) parsing failed");
            }
            if (!(iss >> numberOfForces)) {
                errorMessage(line, "Number of forces parsing failed");
            }

            for (int i = 0; i < numberOfForces; i++) {
                glm::vec3 force;
                if (!(iss >> force.x >> force.y >> force.z)) {
                    errorMessage(line, "Forces");
                }
                forces.push_back(force);
            }

            anchored = static_cast<bool>(anchoredInt);

            PhysicsObject* pObj = new PhysicsObject(pos, forces, rot, mass, anchored, faceSize, modelPath, texturePath, arrowPath);
            pObj->scale = objScale;
            pObj->ScaleSize(objScale);
            pObj->velocity = vel;
            pObj->acceleration = acc;
            physicsObjects.push_back(pObj);
        }
        if (type == "tObj") {
            glm::vec3 pos;
            glm::quat rot;
            float faceSize;
            float scale;
            std::string modelPath;
            std::string texturePath;

            if (!(iss >> pos.x >> pos.y >> pos.z)) {
                errorMessage(line, "Position (pos.x, pos.y, pos.z) parsing failed");
            }
            if (!(iss >> rot.w >> rot.x >> rot.y >> rot.z)) {
                errorMessage(line, "Rotation (rot.w, rot.x, rot.y, rot.z) parsing failed");
            }
            if (!(iss >> faceSize)) {
                errorMessage(line, "FaceSize parsing failed");
            }
            if (!(iss >> scale)) {
                errorMessage(line, "Scale parsing failed");
            }
            if (!(iss >> modelPath >> texturePath)) {
                errorMessage(line, "File paths (modelPath, texturePath) parsing failed");
            }

            TriggerObject* tObj = new TriggerObject(pos, rot, faceSize, modelPath, texturePath);
            tObj->ScaleSize(scale);
            triggerObjects.push_back(tObj);
        }
        else {
            
        }

    }

    // Must load lessons after objects have loaded to avoid uninitialized object errors
    if (lessonModuleID != "N/A") {
        LoadLessonModule(lessonModuleID);
    }
    if (lessonPath != "N/A") {
        LoadLessonFromFile(lessonPath);
    }

    file.close();
}

void Scenario::AssignScenarioSettings() {
    GlobalData::gravity = gravity;
    GlobalData::scale = scale;
    GlobalData::lockedRotation = lockedRotation;
    GlobalData::time = timeScale;
}

void Scenario::LoadLessonFromFile(const std::string& filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::string line;
    std::string strData[2];
    int pointer = -1;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (line == "[Explanation]") {
            pointer = 0;
        }
        if (line == "[Scenario]") {
            pointer = 1;
        }
        if (pointer >= 0) {
            strData[pointer] += "\n" + line;
        }
    }
    explanation = strData[0];
    scenario = strData[1];
}

void Scenario::LoadLessonModule(std::string id) {
    if (id == "Energy") {
        lessonModule = new EnergyLessonModule();
    }
    if (id == "Projectile_Motion") {
        lessonModule = new ProjectileMotionLessonModule();
        ProjectileMotionLessonModule* projectileLesson = dynamic_cast<ProjectileMotionLessonModule*>(lessonModule);
        projectileLesson->Load(physicsObjects[0], triggerObjects[0]);
    }
}

void Scenario::SaveScenarioToFile(const std::string& filepath, const std::string& name, std::vector<PhysicsObject*> PhysicObjects, std::vector<TriggerObject*> TriggerObjects, Scenario scene) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filepath << std::endl;
        return;
    }

    // Scene
    file << "sData ";
    file << name << " ";
    file << GlobalData::gravity.x << " " << GlobalData::gravity.y << " " << GlobalData::gravity.z << " ";
    file << GlobalData::scale << " ";
    file << static_cast<int>(GlobalData::lockedRotation) << " " << GlobalData::time;
    file << std::endl;

    // Lesson (not always applicable)
    std::string lessonTitle = scene.title == "" ? "N/A" : scene.title;
    std::string lessonPath = scene.lessonPath == "" ? "N/A" : scene.lessonPath;
    std::string lessonModuleID = scene.lessonModuleID == "" ? "N/A" : scene.lessonModuleID;
    file << "lData ";
    file << lessonTitle << " ";
    file << lessonPath << " ";
    file << lessonModuleID;
    file << std::endl;

    for (auto pObj : PhysicObjects) {
        file << "pObj ";

        file << pObj->pos.x << " " << pObj->pos.y << " " << pObj->pos.z << " ";
        file << pObj->rot.w << " " << pObj->rot.x << " " << pObj->rot.y << " " << pObj->rot.z << " ";
        file << pObj->velocity.x << " " << pObj->velocity.y << " " << pObj->velocity.z << " ";
        file << pObj->acceleration.x << " " << pObj->acceleration.y << " " << pObj->acceleration.z << " ";

        file << pObj->GetMass() << " ";

        file << static_cast<int>(pObj->IsAnchored()) << " " << pObj->oriignalFaceSize << " ";

        file << pObj->scale << " ";

        file << pObj->modelPath << " " << pObj->texturePath << " " << pObj->arrowPath << " ";

        file << 0;
        file << std::endl;
    }

    for (auto tObj : TriggerObjects) {
        file << "tObj ";

        file << tObj->pos.x << " " << tObj->pos.y << " " << tObj->pos.z << " ";
        file << tObj->rot.w << " " << tObj->rot.x << " " << tObj->rot.y << " " << tObj->rot.z << " ";
        file << tObj->GetFaceExtent() << " " << tObj->scale << " ";

        file << tObj->modelPath << " " << tObj->texturePath << " ";

        file << std::endl;
    }

    file.close();
}

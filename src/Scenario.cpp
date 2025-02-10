#pragma once
#include "Scenario.h"

/*
File Format:
// Scenario Data: sData 
            Name (std::string)

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
*/

void errorMessage(std::string line, std::string specifier) {
    std::cerr << "Error parsing physics object line for \"" << specifier << "\": " << line << std::endl;
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
            if (!(iss >> name)) {
                errorMessage(line, "Scenario name parsing failed");
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
            pObj->velocity = vel;
            pObj->acceleration = acc;
            physicsObjects.push_back(pObj);
        }
        else {
            
        }
    }

    file.close();
}

void Scenario::SaveScenarioToFile(const std::string& filepath, const std::string& name, std::vector<PhysicsObject*> PhysicObjects, std::vector<TriggerObject*> TriggerObjects, glm::vec3 gravity) {
    std::ofstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filepath << std::endl;
        return;
    }

    for (auto pObj : PhysicObjects) {
        file << "pObj ";

        file << pObj->pos.x << " " << pObj->pos.y << " " << pObj->pos.z << " ";
        file << pObj->rot.w << " " << pObj->rot.x << " " << pObj->rot.y << " " << pObj->rot.z << " ";
        file << pObj->velocity.x << " " << pObj->velocity.y << " " << pObj->velocity.z << " ";
        file << pObj->acceleration.x << " " << pObj->acceleration.y << " " << pObj->acceleration.z << " ";

        file << pObj->GetMass() << " ";

        file << static_cast<int>(pObj->IsAnchored()) << " " << pObj->GetFaceExtent() << " ";

        file << pObj->modelPath << " " << pObj->texturePath << " " << pObj->arrowPath << " ";

        file << 0;
        file << std::endl;
    }
    file.close();
}

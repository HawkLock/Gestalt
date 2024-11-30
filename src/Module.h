#pragma once
#include <vector>
#include <iostream>
#include <cstdio> 

#include "PhysicsObject.h" // Make sure to include PhysicsObject header

class Module {
public:
    std::vector<PhysicsObject*> objects;

    virtual void RenderWindow() {
    }

    virtual void UpdateObjects(const std::vector<PhysicsObject*>& updatedObjects) {
        // Avoid object slicing if you are using references or pointers
        objects = updatedObjects;
    }

    void GenerateVectorSubfolder(const char* title, glm::vec3* vec) {
        if (ImGui::TreeNode(title)) {
            ImGui::SliderFloat("X", &vec->x, -100, 100);
            ImGui::SliderFloat("Y", &vec->y, -100, 100);
            ImGui::SliderFloat("Z", &vec->z, -100, 100);
            ImGui::TreePop();
        }
    }

    virtual ~Module() = default; // Virtual destructor to ensure proper cleanup of derived classes
};

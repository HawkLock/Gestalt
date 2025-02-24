#pragma once
#include <vector>
#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class ObjectModule : public Module {
private: 


public:

    ObjectModule() {
    }

    void HandleData(const std::vector<PhysicsObject*>& data) {
        objects = data;
    }

    void GenerateObjectHeader(const char* title, PhysicsObject* object) {

        if (ImGui::TreeNode(title)) {
            GenerateVectorSubfolder("Pos", &object->pos);
            GenerateVectorSubfolder("Velocity", &object->velocity, velocityMin, velocityMax);
            GenerateVectorSubfolder("Acceleration", &object->acceleration, accelerationMin, accelerationMax);

            float energy = object->CalculateTotalEnergy();
            ImGui::Text("Energy: %.2f J", energy);

            ImGui::TreePop();
        }
    }

    void RenderWindow() override {
        //ImGui::Begin("Objects");

        for (int i = 0; i < objects.size(); i++) {
            // Convert i to string and concatenate
            std::string header = "Object " + std::to_string(i);
            GenerateObjectHeader(header.c_str(), objects[i]);
        }

        //ImGui::End();
    }
};

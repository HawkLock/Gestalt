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
        ImGui::Begin("Objects");

        for (int i = 0; i < objects.size(); i++) {
            // Convert i to string and concatenate
            std::string header = "Object " + std::to_string(i);
            GenerateObjectHeader(header.c_str(), objects[i]);
        }


        //// TreeNode for hierarchical folder structure
        //if (ImGui::TreeNode("Advanced Settings")) {
        //    if (ImGui::TreeNode("Subfolder 1")) {
        //        ImGui::Checkbox("Enable Feature A", &enableFeatureA);
        //        ImGui::SliderInt("Option 1", &option1, 0, 100);
        //        ImGui::TreePop();
        //    }
        //    if (ImGui::TreeNode("Subfolder 2")) {
        //        ImGui::Checkbox("Enable Feature B", &enableFeatureB);
        //        ImGui::SliderInt("Option 2", &option2, 0, 100);
        //        ImGui::TreePop();
        //    }
        //    ImGui::TreePop();
        //}

        ImGui::End();
    }
};

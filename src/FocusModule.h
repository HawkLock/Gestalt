#pragma once
#include <vector>
#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class FocusModule : public Module {
private:
    PhysicsObject* focusObject;

public:

    FocusModule() {
    }

    virtual void HandleData(const std::pair<std::string, bool*> data) {
        ImGui::Checkbox(data.first.c_str(), data.second);
    }

    virtual void HandleData(PhysicsObject* data) {
        focusObject = data;
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

    void RenderWindowBody() override {
        if (focusObject == nullptr) {
            return;
        }

        GenerateVectorSubfolder("Pos", &focusObject->pos);
        GenerateVectorSubfolder("Velocity", &focusObject->velocity, velocityMin, velocityMax);
        GenerateVectorSubfolder("Acceleration", &focusObject->acceleration, accelerationMin, accelerationMax);

        float energy = focusObject->CalculateTotalEnergy();
        ImGui::Text("Energy: %.2f J", energy);
    }

    void RenderWindow() override {
        if (focusObject == nullptr) {
            return;
        }

        ImGui::Begin("Focus Object");
        
        GenerateVectorSubfolder("Pos", &focusObject->pos);
        GenerateVectorSubfolder("Velocity", &focusObject->velocity, velocityMin, velocityMax);
        GenerateVectorSubfolder("Acceleration", &focusObject->acceleration, accelerationMin, accelerationMax);

        float energy = focusObject->CalculateTotalEnergy();
        ImGui::Text("Energy: %.2f J", energy);

        ImGui::End();
    }
};

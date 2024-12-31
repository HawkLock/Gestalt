#pragma once
#include <vector>
#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class SettingsModule : public Module {
private:
    std::vector<std::pair<std::string, bool*>> boolSettings;

public:

    SettingsModule() {
        boolSettings = std::vector<std::pair<std::string, bool*>>();
    }

    virtual void HandleData(const std::pair<std::string, bool*> data) {
        ImGui::Checkbox(data.first.c_str(), data.second);
    }

    void GenerateObjectHeader(const char* title, PhysicsObject* object) {

        if (ImGui::TreeNode(title)) {
            // Data To Show Goes Here (idk if this will ever be used)
            ImGui::TreePop();
        }
    }

    void StartRender() {
        ImGui::Begin("Settings Window");
    }

    void EndRender() {
        ImGui::End();
    }

    void RenderWindow() override {
        ImGui::Begin("Settings Window");
        
        //ImGui::Checkbox("Show Arrows", &render->renderArrows);

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

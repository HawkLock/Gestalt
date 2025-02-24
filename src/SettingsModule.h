#pragma once
#include <vector>
#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class SettingsModule : public Module {
private:
    float min;
    float max;
public:

    SettingsModule() {
    }

    virtual void HandleData(const std::pair<std::string, bool*> data) {
        ImGui::Checkbox(data.first.c_str(), data.second);
    }

    virtual void HandleData(const std::pair<float, float> data) {
        min = data.first;
        max = data.second;
    }

    virtual void HandleData(const std::pair<std::string, float*> data) {
        ImGui::SliderFloat(data.first.c_str(), data.second, min, max);
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

        ImGui::End();
    }
};

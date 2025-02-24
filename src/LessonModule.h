#pragma once
#include <vector>
#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Scenario.h"

class LessonModule : public Module {
private: 
    Scenario* scene;

public:

    virtual void HandleData(Scenario* data) {
        scene = data;
    }

    LessonModule() {
    }


    void RenderWindow() {
        ImGui::Begin("Lesson");

        if (scene->title.empty()) {
            ImGui::Text("Error: Lesson could not be loaded.");
        }
        else {

            ImGui::TextWrapped("Lesson: %s", scene->title.c_str());
            ImGui::Separator();

            ImGui::BeginChild("LessonContent", ImVec2(0, 300), true);
            if (ImGui::CollapsingHeader("Explanation")) {
                ImGui::TextWrapped("%s", scene->explanation.c_str());
            }
            if (ImGui::CollapsingHeader("Scenario")) {
                ImGui::TextWrapped("%s", scene->scenario.c_str());
            }
            ImGui::EndChild();
        }

        ImGui::End();
    }

};

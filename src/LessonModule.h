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
    bool* showLessonSubmodule;

public:

    virtual void HandleData(Scenario* data) {
        scene = data;
    }

    virtual void HandleData(bool* data) {
        showLessonSubmodule = data;
    }

    LessonModule() {
    }


    void RenderWindow() {
        ImGuiWindowFlags windowFlag = GlobalData::inFocus ? ImGuiWindowFlags_NoInputs : 0;
        ImGui::Begin("Lesson", nullptr, windowFlag);

        if (showLessonSubmodule != nullptr) {
            ImGui::Checkbox("Show Lesson Submodule", showLessonSubmodule);
        }

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

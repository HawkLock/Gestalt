#pragma once
#include <vector>
#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Scenario.h"

class ScenarioModule : public Module {
private: 


public:
    int selectedScenario = GlobalData::selectedScenario;

    ScenarioModule() {

    }

    void generateScenarioName(std::string* raw) {
        removeSubstring(raw, GlobalData::scenarioPath);
        removeSubstring(raw, ".txt");
    }

    void removeSubstring(std::string* str, std::string substring) {
        std::string::size_type index = str->find(substring);
        if (index != std::string::npos) {
            str->erase(index, substring.length());
        }
    }

    void RenderWindow() override {
        ImGuiWindowFlags windowFlag = GlobalData::inFocus ? ImGuiWindowFlags_NoInputs : 0;
        ImGui::Begin("Scenario Control", nullptr, windowFlag);

        // Dropdown menu to choose a scenario
        std::vector<std::string> availableScenarios = GlobalData::availableScenarios;
        if (availableScenarios.size() == 0) {
            ImGui::End();
            return;
        }

        std::string sceneName = availableScenarios[GlobalData::selectedScenario];
        generateScenarioName(&sceneName);

        if (ImGui::BeginCombo("Select Scenario", sceneName.c_str())) {
            for (int i = 0; i < availableScenarios.size(); ++i) {
                bool isSelected = (selectedScenario == i);
                sceneName = availableScenarios[i];
                generateScenarioName(&sceneName);
                if (ImGui::Selectable(sceneName.c_str(), isSelected)) {
                    selectedScenario = i;
                }
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Load scenario button
        if (ImGui::Button("Scan Available Scenarios")) {
            GlobalData::shouldRescanScenarios = true;
        }

        ImGui::SliderFloat("Time Modifier", &GlobalData::timeModifier, 0, 2);

        ImGui::Checkbox("Paused", &GlobalData::paused);

        // Load scenario button
        if (ImGui::Button("Load Scenario")) {
            GlobalData::selectedScenario = selectedScenario;
            GlobalData::shouldLoadScenario = true;
        }

        // Reload scenario button
        if (ImGui::Button("Reload Scenario")) {
            GlobalData::shouldLoadScenario = true;
        }

        ImGui::End();
    }
};

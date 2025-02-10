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

    void RenderWindow() override {
        ImGui::Begin("Scenario Control");

        // Dropdown menu to choose a scenario
        std::vector<std::string> availableScenarios = GlobalData::availableScenarios;
        if (availableScenarios.size() == 0) {
            ImGui::End();
            return;
        }
        if (ImGui::BeginCombo("Select Scenario", availableScenarios[selectedScenario].c_str())) {
            for (int i = 0; i < availableScenarios.size(); ++i) {
                bool isSelected = (selectedScenario == i);
                if (ImGui::Selectable(availableScenarios[i].c_str(), isSelected)) {
                    selectedScenario = i;
                }
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

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

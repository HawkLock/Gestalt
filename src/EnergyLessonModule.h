#pragma once
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "BaseLessonModule.h"

class EnergyLessonModule : public BaseLessonModule {
private: 
    std::vector<PhysicsObject*> PhysicObjects = std::vector<PhysicsObject*>();
    std::vector<TriggerObject*> TriggerObjects = std::vector<TriggerObject*>();

public:

    void HandleData(std::vector<PhysicsObject*> data) {
        PhysicObjects = data;
    }

    void HandleData(std::vector<TriggerObject*> data) {
        TriggerObjects = data;
    }

    void CalculateEnergy() {
        float netEnergy = 0;
        float netTransEnergy = 0;
        float netRotEnergy = 0;
        std::vector<float> tranEnergies = std::vector<float>();
        std::vector<float> rotEnergies = std::vector<float>();
        std::vector<float> combinedEnergies = std::vector<float>();

        for (int i = 0; i < PhysicObjects.size(); i++) {
            float transEnergy = PhysicObjects[i]->CalculateTranslationalEnergy();
            float rotEnergy = PhysicObjects[i]->CalculateRotationalEnergy();
            float combinedEnergy = transEnergy + rotEnergy;
            tranEnergies.push_back(transEnergy);
            rotEnergies.push_back(rotEnergy);
            combinedEnergies.push_back(combinedEnergy);
            netEnergy += combinedEnergy;
            netTransEnergy += transEnergy;
            netRotEnergy += rotEnergy;
        }
        ImGui::Text("Total Energy: %.3f", netEnergy);
        ImGui::Text("Translational Energy: %.3f", netTransEnergy);
        ImGui::Text("Rotational Energy: %.3f", netRotEnergy);

        for (int i = 0; i < PhysicObjects.size(); i++) {
            std::string objName = "Object " + std::to_string(i);
            if (ImGui::TreeNode(objName.c_str())) {
                float transEnergy = rotEnergies[i];
                float rotEnergy = combinedEnergies[i];
                float combinedEnergy = transEnergy + rotEnergy;
                ImGui::Text("Total Energy: %.3f", combinedEnergy);
                ImGui::Text("Translational Energy: %.3f", transEnergy);
                ImGui::Text("Rotational Energy: %.3f", rotEnergy);

                ImGui::TreePop();
            }
        }
        
    }

    EnergyLessonModule() {
    }

    void RenderWindow() override {
        ImGui::Begin("Energy Lesson Module");

        ImGui::SliderFloat("Global Restitution", &GlobalData::restitution, 0.0f, 1.0f);
        CalculateEnergy();

        ImGui::End();
    }
};

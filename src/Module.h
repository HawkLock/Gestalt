#pragma once
#include <vector>
#include <iostream>
#include <cstdio> 

#include "GlobalData.h"
#include "PhysicsObject.h" 
#include "Scenario.h"

class Module {
protected:
    // For every object slider
    float velocityMin = -5;
    float velocityMax = 5;

    float accelerationMin = -3;
    float accelerationMax = 3;

public:
    std::vector<PhysicsObject*> objects;

    virtual void RenderWindow() {}
    // For more complex data handling
    void StartRender() {}
    void EndRender() {}

    void GenerateVectorSubfolder(const char* title, glm::vec3* vec) {
        if (ImGui::TreeNode(title)) {
            ImGui::SliderFloat("X", &vec->x, -100, 100);
            ImGui::SliderFloat("Y", &vec->y, -100, 100);
            ImGui::SliderFloat("Z", &vec->z, -100, 100);
            ImGui::TreePop();
        }
    }

    //void GenerateVectorSubfolder(const char* title, glm::vec3* vec, float min, float max) {
    //    if (ImGui::TreeNode(title)) {
    //        ImGui::SliderFloat("X", &vec->x, min, max);
    //        ImGui::SliderFloat("Y", &vec->y, min, max);
    //        ImGui::SliderFloat("Z", &vec->z, min, max);
    //        ImGui::TreePop();
    //    }
    //}

    void GenerateVectorSubfolder(const char* title, glm::vec3* vec, float min, float max) {
        if (ImGui::TreeNode(title)) {
            ImGui::SliderFloat("X", &vec->x, min, max);
            ImGui::SliderFloat("Y", &vec->y, min, max);
            ImGui::SliderFloat("Z", &vec->z, min, max);
            float mag = sqrt(pow(vec->x, 2)+ pow(vec->y, 2)+ pow(vec->z, 2));
            ImGui::Text("Mag: %s", std::to_string(mag).c_str());
            ImGui::TreePop();
        }
    }

    void GenerateVectorSubfolderImmutable(const char* title, glm::vec3* vec) {
        if (ImGui::TreeNode(title)) {
            ImGui::Text("%s, ", std::to_string(vec->x).c_str());
            ImGui::SameLine();
            ImGui::Text("%s, ", std::to_string(vec->y).c_str());
            ImGui::SameLine();
            ImGui::Text("%s", std::to_string(vec->z).c_str());
            float mag = sqrt(pow(vec->x, 2) + pow(vec->y, 2) + pow(vec->z, 2));
            ImGui::Text("Mag: %s", std::to_string(mag).c_str());
            ImGui::TreePop();
        }
    }

    template <typename... Args>
    void UpdateData(Args&&... args) {
        auto handle = [this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            this->HandleData(arg);
            };

        (handle(std::forward<Args>(args)), ...);
    }

    // Handlers for different types of data
    virtual void HandleData(const std::vector<PhysicsObject*>& data) {}
    virtual void HandleData(unsigned int* data) {}
    virtual void HandleData(const int data) {}
    virtual void HandleData(const bool data) {}
    virtual void HandleData(bool* data) {}
    virtual void HandleData(const glm::vec3& data) {}
    virtual void HandleData(const std::pair<std::string, bool*> data) {}
    virtual void HandleData(const std::pair<float, float> data) {}
    virtual void HandleData(const std::pair<std::string, float*> data) {}
    virtual void HandleData(PhysicsObject* data) {}
    virtual void HandleData(Scenario* data) {}

    virtual void RenderWindowBody() {}

    virtual ~Module() = default; 
};

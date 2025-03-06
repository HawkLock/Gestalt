#pragma once
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "BaseLessonModule.h"
#include "PhysicsUtility.h"

class ProjectileMotionLessonModule : public BaseLessonModule {
private: 


public:
    static PhysicsObject* physicsObj;
    static TriggerObject* triggerObj;
    static float angle;
    float desiredAngle = angle;
    static float mag;
    float desiredMag = mag;

    static bool triggered;

    static void trigger(std::chrono::steady_clock::time_point currTime, std::chrono::steady_clock::time_point startTime) {
        if (triggered) {
            return;
        }
        triggered = true;
    }

    void Load(PhysicsObject* pObj, TriggerObject* tObj) {
        physicsObj = pObj;
        triggerObj = tObj;
        glm::vec3 vel = glm::vec3(cos(PhysicsUtility::degToRad(angle)) * mag, sin(PhysicsUtility::degToRad(angle)) * mag, 0);
        physicsObj->SetVelocity(vel);
        triggerObj->setTrigger(ProjectileMotionLessonModule::trigger);
    }

    void RenderWindow() override {
        ImGui::Begin("Projectile Motion Lesson Module");

        // Reload scenario button
        if (ImGui::Button("Reload Scenario")) {
            GlobalData::shouldLoadScenario = true;
            triggered = false;
            angle = desiredAngle;
            mag = desiredMag;
        }

        ImGui::Checkbox("Paused", &GlobalData::paused);

        ImGui::SliderFloat("Angle", &desiredAngle, 0, 90);
        ImGui::SliderFloat("Magnitude", &desiredMag, 0, 25);

        if (triggered) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Trigger Pressed!");
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Trigger Not Pressed!");
        }

        ImGui::End();
    }
};

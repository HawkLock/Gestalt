#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <random> 

#include "Gestalt.h"

float angleWithXAxis(const glm::vec3& v) {
    glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
    float dot = glm::dot(glm::normalize(v), xAxis);
    float angle = acosf(glm::clamp(dot, -1.0f, 1.0f)); // radians
    return angle;
}

bool boxTriggerActivated = false;
float boxTriggerTime;

void boxTrigger(std::chrono::steady_clock::time_point currTime, std::chrono::steady_clock::time_point startTime) {
    boxTriggerTime = GlobalData::currentSimulationTime;
    boxTriggerActivated = true;
}

int main()
{
    std::string bluePath = "../Textures/blue.jpeg";
    std::string greenPath = "../Textures/green.jpeg";
    std::string yellowPath = "../Textures/yellow.jpeg";
    std::string grayPath = "../Textures/black.jpeg";

    Gestalt gestalt;

    gestalt.Initialize();

    GlobalData::gravity = glm::vec3(0, -0.98, 0);
    GlobalData::gravityScale = 1;
    GlobalData::time = 1;
    GlobalData::renderArrowsDecomposed = false;
    GlobalData::restitution = 0.5;

    PObj* block = gestalt.CreateObject(CUBE, 0.25, false, bluePath);
    block->SetPosition(glm::vec3(-8, 0, 0));
    block->SetVelocity(glm::vec3(0.75, 5.5, 0));
    block->SetMass(20);
    gestalt.AddObjectToScene(block);

    PObj* wall = gestalt.CreateObject(PLANE, 0.8, true, grayPath);
    wall->SetPosition(glm::vec3(1, 4, 0));
    wall->SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f)));
    gestalt.AddObjectToScene(wall);

    PObj* ceil = gestalt.CreateObject(PLANE, 1, true, grayPath);
    ceil->SetPosition(glm::vec3(2.1, 5, 0));
    ceil->SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
    gestalt.AddObjectToScene(ceil);

    PObj* platform = gestalt.CreateObject(PLANE, 1, true, yellowPath);
    platform->SetPosition(glm::vec3(0, -0.5, 0));
    platform->SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
    gestalt.AddObjectToScene(platform);

    TObj* goal = gestalt.CreateObject(CUBE, 0.2, greenPath);
    goal->SetPosition(glm::vec3(3, 3.75, 0));
    goal->SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
    goal->setTrigger(boxTrigger);
    gestalt.AddObjectToScene(goal);

    float angle = glm::degrees(angleWithXAxis(block->GetVelocity()));

    Widget* widget = gestalt.CreateWidget("Velocities");
    widget->AddTracker("Block Launch Velocity", &block->GetObject()->velocity);
    widget->AddTracker("Block Launch Angle", &angle);
    widget->AddTracker("Box Trigger Activated", &boxTriggerActivated);
    widget->AddTracker("Box Trigger Time", &boxTriggerTime);
    gestalt.AddObjectToScene(widget);

    gestalt.SaveScenario("test");

    gestalt.Run();
}

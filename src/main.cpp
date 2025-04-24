#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <random> 

#include "Gestalt.h"


float GetRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

glm::vec3 GetRandomVelocity() {
    return glm::vec3(GetRandomFloat(-2.0f, 2.0f), GetRandomFloat(1.0f, 5.0f), GetRandomFloat(-2.0f, 2.0f));
}

bool boxTriggerActivated = false;
float boxTriggerTime;

void boxTrigger(std::chrono::steady_clock::time_point currTime, std::chrono::steady_clock::time_point startTime) {
    boxTriggerTime = GlobalData::currentSimulationTime;
    boxTriggerActivated = true;
}

int main()
{
    Gestalt gestalt;

    gestalt.Initialize();

    GlobalData::gravity = glm::vec3(0, -0.98, 0);
    GlobalData::gravityScale = 1;
    GlobalData::time = 1.75;
    GlobalData::renderArrowsDecomposed = true;

    PObj* block = gestalt.CreateObject(CUBE, 0.3, false);
    block->SetPosition(glm::vec3(-8, 0, 0));
    block->SetVelocity(glm::vec3(3, 0, 0));
    block->SetMass(20);
    gestalt.AddObjectToScene(block);

    TObj* line = gestalt.CreateObject(SLIVER, 1);
    line->SetPosition(glm::vec3(8, 0, 0));
    line->SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
    line->setTrigger(boxTrigger);
    gestalt.AddObjectToScene(line);

    Widget* widget = gestalt.CreateWidget("Velocities");
    widget->AddTracker("Block Launch Velocity", &block->GetObject()->velocity, 0, 3);
    widget->AddTracker("Box Trigger Activated", &boxTriggerActivated);
    widget->AddTracker("Box Trigger Time", &boxTriggerTime);
    gestalt.AddObjectToScene(widget);

    gestalt.SaveScenario("test");

    gestalt.Run();
}

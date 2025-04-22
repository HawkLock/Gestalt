#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <random> 

#include "Gestalt.h"

void boxTrigger(std::chrono::steady_clock::time_point currTime, std::chrono::steady_clock::time_point startTime) {
    std::chrono::duration<double> elapsed = currTime - startTime;
    std::cout << "Elapsed Time: " << elapsed.count() << std::endl;
}

float GetRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

glm::vec3 GetRandomVelocity() {
    return glm::vec3(GetRandomFloat(-2.0f, 2.0f), GetRandomFloat(1.0f, 5.0f), GetRandomFloat(-2.0f, 2.0f));
}

int main()
{
    Gestalt gestalt;

    gestalt.Initialize();

    GlobalData::gravityScale = 0;
    GlobalData::restitution = 0;
    GlobalData::time = 0.5;

    PObj* left = gestalt.CreateObject(CUBE, 1, false);
    left->SetPosition(glm::vec3(-3, 0, 0));
    left->SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
    left->SetVelocity(glm::vec3(3, 0, 0));
    left->SetMass(20);
    gestalt.AddObjectToScene(left);

    PObj* right = gestalt.CreateObject(CUBE, 1, false);
    right->SetPosition(glm::vec3(3, 0, 0));
    right->SetVelocity(glm::vec3(-2, 0, 0));
    right->SetMass(10);
    gestalt.AddObjectToScene(right);

    Widget* widget = gestalt.CreateWidget("Velocities");
    widget->AddTracker("Left Velocity", &left->GetObject()->velocity);
    widget->AddTracker("Right Velocity", &right->GetObject()->velocity);
    //gestalt.AddObjectToScene(widget);

    gestalt.SaveScenario("test");

    gestalt.Run();

}

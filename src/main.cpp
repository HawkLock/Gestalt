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
    // Random velocity in x, y, z direction
    return glm::vec3(GetRandomFloat(-2.0f, 2.0f), GetRandomFloat(1.0f, 5.0f), GetRandomFloat(-2.0f, 2.0f));
}

int main()
{
    Gestalt gestalt;

    gestalt.Initialize();

    GlobalData::gravity = glm::vec3(0.0, -9.8, 0.0);
    GlobalData::gravityScale = 1;
    //GlobalData::constantAcceleration = true;

    //int row = 2;
    //int col = 2;
    //float size = 2;
    //for (int i = -1; i < row; i++) {
    //    for (int j = -1; j < col; j++) {
    //        PObj* plane = gestalt.CreateObject(PLANE, size, true);
    //        plane->SetPosition(glm::vec3(i*size*2, -2, j*size*2));
    //        plane->SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
    //        gestalt.AddObjectToScene(plane);
    //    }
    //}
    PObj* plane = gestalt.CreateObject(PLANE, 30, true);
    plane->SetPosition(glm::vec3(0, -2, 0));
    plane->SetRotation(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
    gestalt.AddObjectToScene(plane);

    for (int i = 0; i < 4; i++) {
        // Generate random offsets for x and z within a specified range
        float randomXOffset = GetRandomFloat(-0.005f, 0.005f);  // Adjust the range as needed
        float randomZOffset = GetRandomFloat(-0.005f, 0.005f);  // Adjust the range as needed

        PObj* sphere = gestalt.CreateObject(SPHERE, 2, false);

        // Apply random offsets to x and z, while maintaining the original y position
        sphere->SetPosition(glm::vec3(i * 0.01 + randomXOffset, 1 + 3.5 * i, randomZOffset));

        gestalt.AddObjectToScene(sphere);
    }

    //PObj* obj2 = gestalt.CreateObject(CUBE, 0.1, false);
    //gestalt.AddObjectToScene(obj2);
    //obj2->Scale(0.5f);

    //TObj* tObj = gestalt.CreateObject(CUBE, 0.25);
    //tObj->setTrigger(boxTrigger);
    //gestalt.AddObjectToScene(tObj);

    gestalt.SaveScenario("test");

    gestalt.Run();

}

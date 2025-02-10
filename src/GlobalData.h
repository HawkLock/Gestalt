#pragma once
#include <vector>
#include <iostream>
#include <cstdio> 

#include "PhysicsObject.h" 

class GlobalData {

public:
    static int selectedScenario;
    static bool shouldLoadScenario;
    static std::vector<std::string> availableScenarios;

    static bool paused;

    GlobalData() {
    }
};

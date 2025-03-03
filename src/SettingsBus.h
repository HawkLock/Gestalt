#pragma once
#include <vector>
#include <iostream>
#include <cstdio> 

#include "PhysicsObject.h" 
#include "Scenario.h"

class SettingsBus {

public:
    // Objects
    std::vector<PhysicsObject*> PhysicObjects;
    std::vector<TriggerObject*> TriggerObjects;

    // Focus Object
    PhysicsObject* focusObject;
    bool* followFocusedObject;

    // Arrows
    bool* renderArrows; // Render velocity/acceleration arrows
    bool* decomposeArrows; // Breaks down velocity/acceleration arrows into its components
    bool* renderArrowsOnTop; // Renders velocity/acceleration arrows in front of everything else
    bool* renderArrowLabels; // WIP

    // Scenarios
    Scenario* scene;
};

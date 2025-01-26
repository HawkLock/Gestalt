#pragma once
#include <vector>
#include <iostream>
#include <cstdio> 

#include "PhysicsObject.h" 

class SettingsBus {

public:
    PhysicsObject* focusObject;
    bool* followFocusedObject;

    bool* renderArrows; // Render velocity/acceleration arrows
    bool* decomposeArrows; // Breaks down velocity/acceleration arrows into its components
    bool* renderArrowsOnTop; // Renders velocity/acceleration arrows in front of everything else
};

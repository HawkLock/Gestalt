#pragma once
#include <vector>
#include <iostream>
#include <cstdio> 

#include "PhysicsObject.h" 

class GlobalData {

public:
    // Collision
    static bool useGlobalRestitution;
    static float restitution;
    static float minVelocityThreshold;

    // Scenario Stuff
    static std::string scenarioPath;
    static int selectedScenario;
    static bool shouldLoadScenario;
    static bool shouldRescanScenarios;
    static std::vector<std::string> availableScenarios;

    // Information Passing
    static float totalEnergy;

    // Time
    static float time;
    static bool paused;
    static float timeModifier;
    static float minimumFixedTimeStep;
    static float fixedTimeStep;
    static float currentSimulationTime;

    // Scaling
    static float scale;

    // Settings
    static bool inFocus;
    static glm::vec3 gravity;
    static float gravityScale;
    static bool lockedRotation;
    static bool constantAcceleration;

    static bool renderArrows;
    static bool renderArrowsDecomposed;
    static bool renderArrowsOnTop;
    static bool renderArrowLabels;

    // Recording Settings
    static bool shouldRecord;
    static bool includeGUI;
    static bool includePause;
    static bool recordingUnpauses;


    GlobalData() {
    }
};

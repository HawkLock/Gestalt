#include "GlobalData.h"

bool GlobalData::useGlobalRestitution = true;
float GlobalData::restitution = 1.0;
float GlobalData::minVelocityThreshold = 0.7f;


std::string GlobalData::scenarioPath = "../Scenarios/";
int GlobalData::selectedScenario = 0;
bool GlobalData::shouldLoadScenario = false;
std::vector<std::string> GlobalData::availableScenarios;

float GlobalData::time = 1.0;
bool GlobalData::paused = false;
float GlobalData::timeModifier = 1.0f;
float GlobalData::minimumFixedTimeStep = 0.0001;
float GlobalData::fixedTimeStep = 0.001f;

float GlobalData::scale = 0.1f;

float GlobalData::gravityScale = 1.0f;
bool GlobalData::lockedRotation = true;
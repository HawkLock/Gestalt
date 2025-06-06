#include "GlobalData.h"

bool GlobalData::useGlobalRestitution = true;
float GlobalData::restitution = 1.0;
float GlobalData::minVelocityThreshold = 0.7f;


std::string GlobalData::scenarioPath = "../Scenarios/";
int GlobalData::selectedScenario = 0;
bool GlobalData::shouldLoadScenario = false;
bool GlobalData::shouldRescanScenarios = false;
std::vector<std::string> GlobalData::availableScenarios;

float GlobalData::time = 1.0;
bool GlobalData::paused = true;
float GlobalData::timeModifier = 1.0f;
float GlobalData::minimumFixedTimeStep = 0.0001;
float GlobalData::fixedTimeStep = 0.001f;
float GlobalData::currentSimulationTime = 0;

float GlobalData::scale = 0.4f;

bool GlobalData::inFocus = true;
glm::vec3 GlobalData::gravity = glm::vec3(0.0, -9.8, 0.0);
float GlobalData::gravityScale = 1.0f;
bool GlobalData::lockedRotation = true;
bool GlobalData::constantAcceleration = false;
bool GlobalData::renderArrows = true;
bool GlobalData::renderArrowsDecomposed = false;
bool GlobalData::renderArrowsOnTop = false;
bool GlobalData::renderArrowLabels = false;


bool GlobalData::shouldRecord = false;
bool GlobalData::includeGUI = false;
bool GlobalData::includePause = false;
bool GlobalData::recordingUnpauses = false;
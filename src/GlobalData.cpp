#include "GlobalData.h"

// Define static variables outside the class
int GlobalData::selectedScenario = 0;
bool GlobalData::shouldLoadScenario = false;
std::vector<std::string> GlobalData::availableScenarios;
bool GlobalData::paused = false;

#include "ProjectileMotionLessonModule.h"

PhysicsObject* ProjectileMotionLessonModule::physicsObj = nullptr;
TriggerObject* ProjectileMotionLessonModule::triggerObj = nullptr;
float ProjectileMotionLessonModule::angle = 45;
float ProjectileMotionLessonModule::mag = 14;
bool ProjectileMotionLessonModule::triggered = false;
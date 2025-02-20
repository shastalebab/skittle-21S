#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

extern int target;
extern bool lineTracking;
extern bool jammed;
extern pros::task_t ringsorting;
extern pros::task_t linedetection;

void colorDetect();
void lineDetect(void* test);
void ringsensTask(void* assign);
void unjamTask();
void intakeMove(int target);
void discard();
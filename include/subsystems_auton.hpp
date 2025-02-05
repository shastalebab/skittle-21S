#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

extern bool lineTracking;
extern pros::task_t ringsorting;
extern pros::task_t linedetection;

void colorDetect();
void lineDetect(void* test);
void ringsensTask(void* assign);
void discard();
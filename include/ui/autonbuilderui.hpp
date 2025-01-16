#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

void updatepath();

extern lv_obj_t *builderringind;

extern vector<std::string> modules;

extern bool useredblu;
extern bool useposneg;
extern bool usegoalrush;

void autonbuilderinit();
void autocallback();
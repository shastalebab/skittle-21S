#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

extern int target;
extern bool usingTarget;
extern bool jammed;

enum class AutoMogo {
    OFF = 0,
    PRIMED = 1
};

enum class Colors {
    RED = 0,
    BLUE = 1,
    NEUTRAL = 2,
    SPUR = 3
};

enum class Doinker {
    LEFT = 0,
    RIGHT = 1,
    BOTH = 2
};

void setIntake(int Target);
void setLadyBrown(int Target);
void setMogo(bool state);
void setDoinker(Doinker doinker, bool state);
void setUnjam(bool state);
void tareLadyBrown();

void ladybrownTask();
void colorTask();
void unjamTask();
void distanceTask();

extern AutoMogo mogoState;
extern Colors allianceColor;

void compStatusSet(int mode, int set);
int compPluged();
int compStatus();

/*
0 = disabled
1 = auto
2 = driver
*/

void ledTimeTask();
void ledAllianceTask();
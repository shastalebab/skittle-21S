#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep
#include "okapi/api/units/QAngle.hpp"

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

class Coordinate {
    public: 
        double x;
        double y;
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
int compStatus(int comp);

void ledTimeTask();
void ledAllianceTask();

okapi::QLength findDistance(Coordinate point1, Coordinate point2, ez::drive_directions direction);
okapi::QAngle findAngle(Coordinate point1, Coordinate point2, ez::drive_directions direction);

void moveToPoint(Coordinate currentpoint, Coordinate newpoint, int speed, ez::drive_directions direction);
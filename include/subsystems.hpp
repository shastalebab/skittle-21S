#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

// Motors, sensors, etc
inline pros::Optical ringsens(2);
inline pros::MotorGroup intake({-16, -17});
inline pros::Motor intakefirst(-17);
inline pros::Motor ladybrown(10);
inline ez::Piston mogomech('H');
inline ez::Piston intakeLevel('G');
inline ez::Piston doinker('F');
// inline pros::adi::DigitalIn limit_switch('A');

// Functions
void setIntake();
void setLadyBrown();
void setMogo();
void setDoinker();
bool shift();
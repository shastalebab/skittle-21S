#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

// Motors, sensors, etc
inline pros::Optical ringsens(2);
inline pros::MotorGroup intake({-16, -17});
inline pros::Motor intakefirst(-16);
inline pros::Motor intakesecond(-17);
inline pros::Motor ladybrown(10);
inline ez::Piston mogomech('H');
inline ez::Piston intakeLevel('G');
inline ez::Piston doinkerR('F');
inline ez::Piston doinkerL('E');
inline ez::PID lbPID{0.45, 0, 0, 0, "ladybrown"};
//inline pros::adi::AnalogIn linesafety('E');
// inline pros::adi::DigitalIn limit_switch('A');

// Functions
void setIntake();
void setLadyBrown();
void setMogo();
void setDoinkerR();
void setDoinkerL();
void ladybrownTask();
bool shift();

extern bool setLB;
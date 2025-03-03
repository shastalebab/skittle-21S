#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep
#include "pros/distance.hpp"

// Motors, sensors, etc
inline pros::Optical ringsens(2);
inline pros::Distance distsens(3);
inline pros::MotorGroup intake({-16, -17});
inline pros::Motor intakefirst(-16);
inline pros::Motor intakesecond(-17);
inline pros::Motor ladybrown(10);
inline ez::Piston mogomech('H');
inline ez::Piston intakeLevel('G');
inline ez::Piston doinkerR('F');
inline ez::Piston doinkerL('E');
inline ez::PID lbPID{1.35, .04, .75, 0, "ladybrown"};
//inline pros::adi::AnalogIn linesafety('E');
// inline pros::adi::DigitalIn limit_switch('A');

// Functions
void opcontrolIntake();
void opcontrolLadyBrown();
void opcontrolMogo();
void opcontrolDoinkerR();
void opcontrolDoinkerL();
bool shift();

extern bool setLB;
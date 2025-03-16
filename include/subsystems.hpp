#pragma once

#include <cstdint>
#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep
#include "pros/distance.hpp"
#include "pros/adi.hpp"
#include "stormlib/api.hpp"

// Motors
inline pros::Motor intakefirst(-16);
inline pros::Motor intakesecond(-17);
inline pros::MotorGroup intake({-16, -17});
inline pros::Motor ladybrown(10);
//sensors
inline pros::Optical ringsens(2);
inline pros::Distance distsens(1);
inline pros::adi::LineSensor hooksens('D');
//Pistons
inline ez::Piston doinkerL('E');
inline ez::Piston doinkerR('F');
inline ez::Piston intakeLevel('G');
inline ez::Piston mogomech('H');
//EZ PID
inline ez::PID lbPID{1.35, .04, .75, 0, "ladybrown"};

// Functions
void opcontrolIntake();
void opcontrolLadyBrown();
void opcontrolMogo();
void opcontrolDoinkerR();
void opcontrolDoinkerL();

extern bool setLB;

//LEDs
inline stormlib::clock driverClock;
inline stormlib::aRGB driveLeftLED(4, 1, 40);
inline stormlib::aRGB driveRightLED(4, 2, 40);
inline stormlib::aRGB_manager LEDmanager(&driveLeftLED, &driveRightLED, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
//inline pros::adi::Led leftDriveLED({4, 'a'}, 20);
//inline pros::adi::Led rightDriveLED({4, 'b'}, 20);

#define RED_HEX 0xFF000F
#define BLUE_HEX 0x0C91C9
#define WHITE_HEX 0xa1a1a1
#define GREEN_HEX 0x099e36
#define SPURFLY_HEX 0xB5C15F
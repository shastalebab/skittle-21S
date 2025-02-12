#include "EZ-Template/util.hpp"
#include "main.h"  // IWYU pragma: keep
#include "subsystems.hpp"
#include "subsystems_auton.hpp"	 // IWYU pragma: keep

/////
// For installation, upgrading, documentations, and tutorials, check out our
// website! https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 127;
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;
const int GRAB_MOGO = 55;

///
// Constants
///
void default_constants() {
	// P, I, D, and Start I
	// https://ez-robotics.github.io/EZ-Template/tutorials/tuning_constants
	chassis.pid_drive_constants_set(20.0, 0.0, 100.0);		   // Fwd/rev constants, used for odom and non odom motions
	chassis.pid_heading_constants_set(11.0, 0.0, 20.0);		   // Holds the robot straight while going forward without odom
	chassis.pid_turn_constants_set(3.0, 0.05, 20.0, 15.0);	   // Turn in place constants
	chassis.pid_swing_constants_set(6.0, 0.0, 65.0);		   // Swing constants
	chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);	   // Angular control for odom motions
	chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

	// Exit conditions
	// https://ez-robotics.github.io/EZ-Template/tutorials/tuning_exit_conditions
	chassis.pid_turn_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
	chassis.pid_swing_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
	chassis.pid_drive_exit_condition_set(80_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
	chassis.pid_odom_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 750_ms);
	chassis.pid_odom_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 750_ms);
	chassis.pid_turn_chain_constant_set(3_deg);
	chassis.pid_swing_chain_constant_set(5_deg);
	chassis.pid_drive_chain_constant_set(3_in);

	// Slew constants
	// https://ez-robotics.github.io/EZ-Template/tutorials/slew_constants
	chassis.slew_turn_constants_set(3_deg, 70);
	chassis.slew_drive_constants_set(7_in, 70);
	chassis.slew_swing_constants_set(3_in, 80);

	// The amount that turns are prioritized over driving in odom motions
	// - if you have tracking wheels, you can run this higher.  1.0 is the max
	chassis.odom_turn_bias_set(1.0);

	chassis.odom_look_ahead_set(7_in);			 // This is how far ahead in the path the robot looks at
	chassis.odom_boomerang_distance_set(16_in);	 // This sets the maximum distance away from target that the carrot point can be
	chassis.odom_boomerang_dlead_set(0.625);	 // This handles how aggressive the end of boomerang motions are

	chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}

pros::task_t linedetection;

void move_forward() {
	chassis.odom_xyt_set(0_in, 20_in, 0_deg);
	// CHANGE THIS MOVEMENT BEFORE STATES, its only 72 inches so i can test line detection
	chassis.pid_odom_set({{0_in, 72_in}, fwd, 127});
	chassis.pid_wait();
}

// RED

void testautonRed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
}

void red_5greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	chassis.pid_wait();
	chassis.pid_odom_set({{{96_in, 48_in}, rev, 90}, {{120_in, 48_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	pros::delay(500);
	chassis.pid_odom_set({{130_in, 18_in}, fwd, 90});
	doinkerR.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{90_in, 22_in}, fwd, 60, ez::cw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void red_5ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(88_in, 12_in, 90_deg);
	intakeLevel.set(false);
	chassis.pid_odom_set({{78_in, 11_in}, rev, 60});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);
	pros::delay(500);
	chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, 100);
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{96_in, 48_in}, rev, 100});
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 100}, {{130_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	doinkerR.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{90_in, 22_in}, fwd, 60, ez::cw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void red_gr_corner() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(132_in, 20_in, 0_deg);
	doinkerL.set(true);
	chassis.pid_odom_set({{{128_in, 56_in}, fwd, 127}, {{128_in, 27_in}, rev, 100}});
	chassis.pid_wait_until_index_started(1);
	doinkerL.set(false);
	chassis.pid_wait();
	chassis.pid_odom_set({{{96_in, 48_in}, rev, 90}, {{120_in, 48_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	pros::delay(500);
	mogomech.set(false);
	chassis.pid_odom_set({{{128_in, 48_in}, rev, 60}, {{130_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	doinkerL.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{90_in, 22_in}, fwd, 60, ez::cw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void red_gr_wp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(132_in, 20_in, 0_deg);
	doinkerL.set(true);
	chassis.pid_odom_set({{{128_in, 56_in}, fwd, 127}, {{128_in, 27_in}, rev, 100}});
	chassis.pid_wait_until_index_started(1);
	doinkerL.set(false);
	chassis.pid_wait();
	chassis.pid_odom_set({{80_in, 14_in}, fwd, 90});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);
	pros::delay(500);
	chassis.pid_odom_set({{{96_in, 48_in}, rev, 90}, {{120_in, 48_in}, fwd, 127}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	pros::delay(500);
	mogomech.set(false);
	chassis.pid_odom_set({{{128_in, 48_in}, rev, 60}, {{130_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	doinkerL.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{90_in, 22_in}, fwd, 60, ez::cw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void red_6ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	intake.move(119);
	ladybrown.move_relative(600, 200);
	chassis.pid_odom_set({{48_in, 48_in}, rev, 100});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{44_in, 50_in}, fwd, 100}, {{37_in, 61_in}, fwd, 80}, {{16_in, 63_in}, fwd, 80}});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);

	pros::delay(750);
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 100}, {{14_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	doinkerR.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 60, ez::ccw});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void red_7greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(56_in, 12_in, 270_deg);
	chassis.pid_odom_set({{66_in, 11_in}, rev, 60});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);
	pros::delay(500);
	chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, 100);
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{48_in, 48_in}, rev, 100});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	chassis.pid_odom_set({{{44_in, 50_in}, fwd, 100}, {{37_in, 61_in}, fwd, 80}, {{16_in, 63_in}, fwd, 80}, {{24_in, 48_in}, fwd, 100}, {{14_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(4);
	doinkerR.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{54_in, 22_in}, fwd, 60, ez::ccw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void red_7ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(56_in, 12_in, 90_deg);
	intakeLevel.set(false);
	intake.move(119);
	ladybrown.move_relative(600, 200);
	chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, 100, 25);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(500);
	chassis.pid_odom_set(-12.25_in, 70, false);
	chassis.pid_wait();
	pros::delay(500);
	chassis.odom_xyt_set(72_in, 15_in, 0_deg);
	chassis.pid_odom_set({{{72_in, 20_in}, fwd, 60}, {{48_in, 48_in}, rev, 100}});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{44_in, 50_in}, fwd, 100}, {{37_in, 61_in}, fwd, 80}, {{16_in, 63_in}, fwd, 80}});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);

	pros::delay(750);
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 100}, {{14_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	doinkerR.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 60, ez::ccw});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

// BLUE

void testautonBlue() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
}

void blue_5greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	chassis.pid_wait();
	chassis.pid_odom_set({{{48_in, 48_in}, rev, 90}, {{24_in, 48_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	pros::delay(500);
	chassis.pid_odom_set({{14_in, 18_in}, fwd, 90});
	doinkerL.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{54_in, 22_in}, fwd, 60, ez::ccw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void blue_5ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(56_in, 12_in, 90_deg);
	intakeLevel.set(false);
	chassis.pid_odom_set({{66_in, 11_in}, rev, 60});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);
	pros::delay(500);
	chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, 100);	
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{48_in, 48_in}, rev, 100});
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 100}, {{14_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	doinkerR.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{54_in, 22_in}, fwd, 60, ez::ccw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}


void blue_gr_corner() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(12_in, 20_in, 0_deg);
	doinkerR.set(true);
	chassis.pid_odom_set({{{16_in, 56_in}, fwd, 127}, {{16_in, 27_in}, rev, 100}});
	chassis.pid_wait_until_index_started(1);
	doinkerR.set(false);
	chassis.pid_wait();
	chassis.pid_odom_set({{{48_in, 48_in}, rev, 90}, {{24_in, 48_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	pros::delay(500);
	mogomech.set(false);
	chassis.pid_odom_set({{{16_in, 48_in}, rev, 60}, {{14_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	doinkerR.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{54_in, 22_in}, fwd, 60, ez::ccw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void blue_gr_wp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(12_in, 20_in, 0_deg);
	doinkerR.set(true);
	chassis.pid_odom_set({{{16_in, 56_in}, fwd, 127}, {{16_in, 27_in}, rev, 100}});
	chassis.pid_wait_until_index_started(1);
	doinkerR.set(false);
	chassis.pid_wait();
	chassis.pid_odom_set({{64_in, 14_in}, fwd, 90});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);
	pros::delay(500);
	chassis.pid_odom_set({{{48_in, 48_in}, rev, 90}, {{24_in, 48_in}, fwd, 127}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	pros::delay(500);
	mogomech.set(false);
	chassis.pid_odom_set({{{16_in, 48_in}, rev, 60}, {{14_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	mogomech.set(true);
	doinkerR.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{54_in, 22_in}, fwd, 60, ez::ccw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void blue_6ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	intake.move(119);
	ladybrown.move_relative(600, 200);
	chassis.pid_odom_set({{96_in, 48_in}, rev, 100});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{100_in, 50_in}, fwd, 100}, {{107_in, 61_in}, fwd, 80}, {{128_in, 63_in}, fwd, 80}});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);

	pros::delay(750);
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 100}, {{130_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	doinkerL.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 60, ez::cw});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void blue_7greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(88_in, 12_in, 90_deg);
	chassis.pid_odom_set({{78_in, 11_in}, rev, 60});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);
	pros::delay(500);
	chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, 100);
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{96_in, 48_in}, rev, 100});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
	chassis.pid_odom_set({{{100_in, 50_in}, fwd, 100}, {{107_in, 61_in}, fwd, 80}, {{128_in, 63_in}, fwd, 80}, {{120_in, 48_in}, fwd, 100}, {{130_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(4);
	doinkerL.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{90_in, 22_in}, fwd, 60, ez::cw}, {{72_in, 24_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void blue_7ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(88_in, 12_in, 270_deg);
	intakeLevel.set(false);
	intake.move(119);
	ladybrown.move_relative(600, 200);
	chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, 100, 25);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(500);
	chassis.pid_odom_set(-12.25_in, 70, false);
	chassis.pid_wait();
	pros::delay(500);
	chassis.odom_xyt_set(72_in, 15_in, 0_deg);
	chassis.pid_odom_set({{{72_in, 20_in}, fwd, 60}, {{96_in, 48_in}, rev, 100}});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{{100_in, 50_in}, fwd, 100}, {{107_in, 61_in}, fwd, 80}, {{128_in, 63_in}, fwd, 80}});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);

	pros::delay(750);
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 100}, {{130_in, 18_in}, fwd, 90}});
	chassis.pid_wait_until_index_started(1);
	doinkerL.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 60, ez::cw});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

// SKILLS

void skills() {}

///
// Drive Example
///
void drive_example() {
	// The first parameter is target inches
	// The second parameter is max speed the robot will drive at
	// The third parameter is a boolean (true or false) for enabling/disabling a
	// slew at the start of drive motions for slew, only enable it when the drive
	// distance is greater than the slew distance + a few inches

	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait();

	chassis.pid_drive_set(-12_in, DRIVE_SPEED);
	chassis.pid_wait();

	chassis.pid_drive_set(-12_in, DRIVE_SPEED);
	chassis.pid_wait();
}

///
// Turn Example
///
void turn_example() {
	// The first parameter is the target in degrees
	// The second parameter is max speed the robot will drive at

	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_turn_set(45_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait();

	chassis.pid_turn_set(45_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_turn_set(-45_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
	// pid_wait_until will wait until the robot gets to a desired position

	// When the robot gets to 6 inches slowly, the robot will travel the remaining
	// distance at full speed
	chassis.pid_drive_set(24_in, 30, true);
	chassis.pid_wait_until(6_in);
	chassis.pid_speed_max_set(DRIVE_SPEED);	 // After driving 6 inches at 30 speed, the robot will go the
											 // remaining distance at DRIVE_SPEED
	chassis.pid_wait();

	chassis.pid_turn_set(45_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_turn_set(-45_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait();

	// When the robot gets to -6 inches slowly, the robot will travel the
	// remaining distance at full speed
	chassis.pid_drive_set(-24_in, 30, true);
	chassis.pid_wait_until(-6_in);
	chassis.pid_speed_max_set(DRIVE_SPEED);	 // After driving 6 inches at 30 speed, the robot will go the
											 // remaining distance at DRIVE_SPEED
	chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
	// The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
	// The second parameter is the target in degrees
	// The third parameter is the speed of the moving side of the drive
	// The fourth parameter is the speed of the still side of the drive, this
	// allows for wider arcs

	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
	chassis.pid_wait();

	chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
	chassis.pid_wait();

	chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
	chassis.pid_wait();

	chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
	chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
	// Motion chaining is where motions all try to blend together instead of
	// individual movements. This works by exiting while the robot is still moving
	// a little bit. To use this, replace pid_wait with pid_wait_quick_chain.
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait();

	chassis.pid_turn_set(45_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();

	chassis.pid_turn_set(-45_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();

	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait();

	// Your final motion should still be a normal pid_wait
	chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait();

	chassis.pid_turn_set(45_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
	chassis.pid_wait();

	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait();

	chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts) {
	for(int i = 0; i < attempts - 1; i++) {
		// Attempt to drive backward
		printf("i - %i", i);
		chassis.pid_drive_set(-12_in, 127);
		chassis.pid_wait();

		// If failsafed...
		if(chassis.interfered) {
			chassis.drive_sensor_reset();
			chassis.pid_drive_set(-2_in, 20);
			pros::delay(1000);
		}
		// If the robot successfully drove back, return
		else {
			return;
		}
	}
}

// If there is no interference, the robot will drive forward and turn 90
// degrees. If interfered, the robot will drive forward and then attempt to
// drive backward.
void interfered_example() {
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait();

	if(chassis.interfered) {
		tug(3);
		return;
	}

	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();
}

// . . .
// Make your own autonomous functions here!
// . . .
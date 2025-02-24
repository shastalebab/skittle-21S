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
	intakeMove(127);
	chassis.pid_wait();
}

void red_5greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_odom_set({{96_in, 48_in}, rev, 60});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 70});
	chassis.pid_wait();
	pros::delay(250);
	// sweep and score corner
	chassis.pid_odom_set({{134_in, 10_in}, fwd, 90});
	chassis.pid_wait();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab mid top ring
	chassis.pid_odom_set({{74_in, 22_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(500);
	// drop goal in corner
	chassis.pid_odom_set({{138_in, 6_in}, rev, 90});
	chassis.pid_wait();
	mogomech.set(false);
	// touch ladder
	chassis.pid_odom_set({{72_in, 44_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void red_5ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload
	chassis.pid_turn_set({80_in, 12_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{80_in, 12_in}, fwd, 60});
	chassis.pid_wait();
	ladybrown.move_relative(1000, 200);
	pros::delay(250);
	// grab mogo
	chassis.pid_odom_set({{101_in, 50_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// grab bottom ring of ring stack, then sweep corner
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{134_in, 10_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab mid top ring
	chassis.pid_odom_set({{74_in, 22_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(500);
	// drop goal in corner
	chassis.pid_odom_set({{138_in, 6_in}, rev, 90});
	chassis.pid_wait();
	mogomech.set(false);
	// touch ladder
	chassis.pid_odom_set({{72_in, 44_in}, fwd, 90});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void red_gr_corner() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(132_in, 20_in, 0_deg);
	// rush mogo
	doinkerL.set(true);
	chassis.pid_odom_set({{{129_in, 44_in}, fwd, 127}, {{128_in, 58_in}, fwd, 60}});
	chassis.pid_wait_quick_chain();
	doinkerL.set(false);
	chassis.pid_odom_set({{128_in, 27_in}, rev, 80});
	chassis.pid_wait();
	// turn around and regrab mogo with clamp
	pros::delay(200);
	chassis.pid_turn_relative_set(180_deg, 127);
	pros::delay(300);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{122_in, 48_in}, rev, 70});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	pros::delay(200);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{114_in, 52_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	pros::delay(250);
	// drop mogo and grab other mogo
	mogomech.set(false);
	chassis.pid_odom_set({{92_in, 38_in}, rev, 127});
	chassis.pid_wait_quick();
	mogomech.set(true);
	// sweep and score corner
	chassis.pid_odom_set({{134_in, 10_in}, fwd, 90});
	chassis.pid_wait();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab top ring at mid
	chassis.pid_odom_set({{72_in, 12_in}, fwd, 80});
	pros::delay(250);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	// touch ladder
	chassis.pid_odom_set({{72_in, 44_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void red_gr_wp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(132_in, 20_in, 0_deg);
	// rush mogo
	doinkerL.set(true);
	chassis.pid_odom_set({{{129_in, 44_in}, fwd, 127}, {{128_in, 58_in}, fwd, 60}});
	chassis.pid_wait_quick_chain();
	doinkerL.set(false);
	chassis.pid_odom_set({{128_in, 27_in}, rev, 80});
	chassis.pid_wait();
	// turn around and regrab mogo with clamp
	pros::delay(200);
	chassis.pid_turn_relative_set(180_deg, 127);
	pros::delay(300);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{122_in, 48_in}, rev, 70});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	pros::delay(200);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{114_in, 52_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	pros::delay(250);
	// drop mogo and grab other mogo
	mogomech.set(false);
	chassis.pid_odom_set({{92_in, 38_in}, rev, 127});
	chassis.pid_wait_quick();
	mogomech.set(true);
	// sweep and score corner
	chassis.pid_odom_set({{134_in, 10_in}, fwd, 90});
	chassis.pid_wait();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab top ring at mid
	chassis.pid_odom_set({{72_in, 12_in}, fwd, 80});
	pros::delay(250);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	// score on alliance stake using ladybrown mounting
	chassis.pid_odom_set({{72_in, 2_in}, fwd, 127});
	chassis.pid_wait_quick();
	ladybrown.move_relative(1000, 200);
	pros::delay(250);
	// touch ladder
	chassis.pid_odom_set({{87_in, 60_in}, rev, 127});
	pros::c::task_delete(ringsorting);
}

void red_possolowp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload
	chassis.pid_turn_set({79_in, 12_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{79_in, 9_in}, fwd, 60});
	chassis.pid_wait();
	ladybrown.move_relative(1000, 200);
	pros::delay(250);
	// grab mogo & bottom ring
	chassis.pid_odom_set({{98_in, 48_in}, rev, 80});
	chassis.pid_wait_quick();
	mogomech.set(true);
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// grab mid top ring & drop mogo
	chassis.pid_odom_set({{74_in, 22_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(650);
	mogomech.set(false);
	pros::delay(100);
	// grab mogo
	chassis.pid_odom_set({{43_in, 50_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{44_in, 49_in}, fwd, 90}, {{37_in, 61_in}, fwd, 70}, {{10_in, 61_in}, fwd, 60}});
	chassis.pid_wait();
	pros::delay(500);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// touch ladder
	chassis.pid_turn_set(270_deg, 90);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(60_in, 90, true);
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void red_6ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_odom_set({{48_in, 48_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{44_in, 51_in}, fwd, 90}, {{37_in, 62_in}, fwd, 70}, {{10_in, 63_in}, fwd, 60}});
	chassis.pid_wait();
	// grab bottom ring of ring stack, then sweep corner
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{10_in, 10_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_turn_set({0_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// touch ladder
	chassis.pid_odom_set({{72_in, 44_in}, fwd, 90});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void red_7greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload
	chassis.pid_turn_set({65_in, 12_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{65_in, 9_in}, fwd, 80});
	chassis.pid_wait();
	ladybrown.move_absolute(1000, 200);
	pros::delay(750);
	ladybrown.move_absolute(10, 200);
	// grab mogo
	chassis.pid_odom_set({{48_in, 48_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{43_in, 49_in}, fwd, 90}, {{37_in, 61_in}, fwd, 70}, {{10_in, 61_in}, fwd, 60}});
	chassis.pid_wait();
	// grab bottom ring of ring stack, then sweep corner
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{8_in, 12_in}, fwd, 127}});
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab mid top ring
	chassis.pid_odom_set({{74_in, 20_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(500);
	// ram corner
	chassis.pid_odom_set({{138_in, 10_in}, rev, 90});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void red_7ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(56_in, 12_in, 90_deg);
	// prime actuated intake and grab top ring
	intakeLevel.set(false);
	intakeMove(127);
	ladybrown.move_relative(600, 200);
	chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, 100, 25);
	chassis.pid_wait();
	intakeLevel.set(true);
	// score top ring on alliance stake
	chassis.pid_odom_set(-12.5_in, 80, false);
	chassis.pid_wait();
	pros::delay(500);
	// reset position, back up to not hit wall, and move to + grab mogo
	chassis.odom_xyt_set(72_in, 15_in, 0_deg);
	chassis.pid_odom_set({{{72_in, 30_in}, fwd, 90}, {{48_in, 50_in}, rev, 127}});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	// sweep ring rush rings and score preload on wallstake
	chassis.pid_odom_set({{{44_in, 51_in}, fwd, 90}, {{37_in, 62_in}, fwd, 70}, {{10_in, 63_in}, fwd, 60}});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);
	pros::delay(750);
	// grab bottom ring of ring stack, then sweep corner
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{10_in, 10_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_turn_set({0_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// touch ladder
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void red_negsolowp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload
	chassis.pid_turn_set({65_in, 12_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{65_in, 9_in}, fwd, 60});
	chassis.pid_wait();
	ladybrown.move_relative(1000, 200);
	pros::delay(250);
	// grab mogo
	chassis.pid_odom_set({{43_in, 50_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{44_in, 49_in}, fwd, 90}, {{37_in, 61_in}, fwd, 70}, {{10_in, 61_in}, fwd, 60}});
	chassis.pid_wait();
	pros::delay(500);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// grab mid top ring & drop mogo
	chassis.pid_odom_set({{74_in, 22_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(650);
	mogomech.set(false);
	pros::delay(100);
	// grab other mogo & grab other bottom ring
	chassis.pid_odom_set({{98_in, 48_in}, rev, 80});
	chassis.pid_wait_quick();
	mogomech.set(true);
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// touch ladder
	chassis.pid_turn_set(270_deg, 90);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(60_in, 90, true);
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

// BLUE

void testautonBlue() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
}

void blue_5greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_odom_set({{48_in, 48_in}, rev, 60});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 70});
	chassis.pid_wait();
	pros::delay(250);
	// sweep and score corner
	chassis.pid_odom_set({{10_in, 10_in}, fwd, 90});
	chassis.pid_wait();
	chassis.pid_turn_set({0_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab mid top ring
	chassis.pid_odom_set({{70_in, 22_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(500);
	// drop goal in corner
	chassis.pid_odom_set({{6_in, 6_in}, rev, 90});
	chassis.pid_wait();
	mogomech.set(false);
	// touch ladder
	chassis.pid_odom_set({{72_in, 44_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void blue_5ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload
	chassis.pid_turn_set({64_in, 12_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{64_in, 12_in}, fwd, 60});
	chassis.pid_wait();
	ladybrown.move_relative(1000, 200);
	pros::delay(250);
	// grab mogo
	chassis.pid_odom_set({{43_in, 50_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// grab bottom ring of ring stack, then sweep corner
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{10_in, 10_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_turn_set({0_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab mid top ring
	chassis.pid_odom_set({{70_in, 22_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(500);
	// drop goal in corner
	chassis.pid_odom_set({{6_in, 6_in}, rev, 90});
	chassis.pid_wait();
	mogomech.set(false);
	// touch ladder
	chassis.pid_odom_set({{72_in, 44_in}, fwd, 90});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void blue_gr_corner() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(12_in, 20_in, 0_deg);
	// rush mogo
	doinkerR.set(true);
	chassis.pid_odom_set({{{15_in, 44_in}, fwd, 127}, {{16_in, 58_in}, fwd, 60}});
	chassis.pid_wait_quick_chain();
	doinkerR.set(false);
	chassis.pid_odom_set({{16_in, 27_in}, rev, 80});
	chassis.pid_wait();
	// turn around and regrab mogo with clamp
	pros::delay(200);
	chassis.pid_turn_relative_set(180_deg, 127);
	pros::delay(300);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{22_in, 48_in}, rev, 70});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	pros::delay(200);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{30_in, 52_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	pros::delay(250);
	// drop mogo and grab other mogo
	mogomech.set(false);
	chassis.pid_odom_set({{52_in, 38_in}, rev, 127});
	chassis.pid_wait_quick();
	mogomech.set(true);
	// sweep and score corner
	chassis.pid_odom_set({{10_in, 10_in}, fwd, 90});
	chassis.pid_wait();
	chassis.pid_turn_set({0_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab top ring at mid
	chassis.pid_odom_set({{72_in, 12_in}, fwd, 80});
	pros::delay(250);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	// touch ladder
	chassis.pid_odom_set({{72_in, 44_in}, fwd, 127});
	pros::c::task_delete(ringsorting);
}

void blue_gr_wp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(12_in, 20_in, 0_deg);
	// rush mogo
	doinkerR.set(true);
	chassis.pid_odom_set({{{15_in, 44_in}, fwd, 127}, {{16_in, 58_in}, fwd, 60}});
	chassis.pid_wait_quick_chain();
	doinkerR.set(false);
	chassis.pid_odom_set({{16_in, 27_in}, rev, 80});
	chassis.pid_wait();
	// turn around and regrab mogo with clamp
	pros::delay(200);
	chassis.pid_turn_relative_set(180_deg, 127);
	pros::delay(300);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{22_in, 48_in}, rev, 70});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	pros::delay(200);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{30_in, 52_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	pros::delay(250);
	// drop mogo and grab other mogo
	mogomech.set(false);
	chassis.pid_odom_set({{52_in, 38_in}, rev, 127});
	chassis.pid_wait_quick();
	mogomech.set(true);
	// sweep and score corner
	chassis.pid_odom_set({{10_in, 10_in}, fwd, 90});
	chassis.pid_wait();
	chassis.pid_turn_set({0_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab top ring at mid
	chassis.pid_odom_set({{72_in, 12_in}, fwd, 80});
	pros::delay(250);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	// score on alliance stake using ladybrown mounting
	chassis.pid_odom_set({{72_in, 2_in}, fwd, 127});
	chassis.pid_wait_quick();
	ladybrown.move_relative(1000, 200);
	pros::delay(250);
	// touch ladder
	chassis.pid_odom_set({{57_in, 60_in}, rev, 127});
	pros::c::task_delete(ringsorting);
}

void blue_possolowp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload
	chassis.pid_turn_set({65_in, 12_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{65_in, 9_in}, fwd, 60});
	chassis.pid_wait();
	ladybrown.move_relative(1000, 200);
	pros::delay(250);
	// grab mogo & bottom ring
	chassis.pid_odom_set({{46_in, 48_in}, rev, 80});
	chassis.pid_wait_quick();
	mogomech.set(true);
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// grab mid top ring & drop mogo
	chassis.pid_odom_set({{70_in, 22_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(650);
	mogomech.set(false);
	pros::delay(100);
	// grab other mogo
	chassis.pid_odom_set({{101_in, 50_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{100_in, 49_in}, fwd, 90}, {{107_in, 61_in}, fwd, 70}, {{134_in, 61_in}, fwd, 60}});
	chassis.pid_wait();
	pros::delay(500);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// touch ladder
	chassis.pid_turn_set(90_deg, 90);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(60_in, 90, true);
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void blue_6ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_odom_set({{96_in, 48_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// sweep ring rush rings and score preload on wallstake
	chassis.pid_odom_set({{{100_in, 51_in}, fwd, 90}, {{107_in, 62_in}, fwd, 70}, {{134_in, 63_in}, fwd, 60}});
	chassis.pid_wait();
	// grab bottom ring of ring stack, then sweep corner
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{134_in, 10_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 90);
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// touch ladder
	chassis.pid_odom_set({{72_in, 44_in}, fwd, 90});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void blue_7greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload
	chassis.pid_turn_set({79_in, 12_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{79_in, 9_in}, fwd, 80});
	chassis.pid_wait();
	ladybrown.move_absolute(1000, 200);
	pros::delay(250);
	// grab mogo
	chassis.pid_odom_set({{96_in, 48_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	ladybrown.move_absolute(10, 1000);
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{101_in, 49_in}, fwd, 90}, {{107_in, 61_in}, fwd, 70}, {{134_in, 61_in}, fwd, 60}});
	chassis.pid_wait();
	// grab bottom ring of ring stack, then sweep corner
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{136_in, 12_in}, fwd, 127}});
	chassis.pid_wait();
	ram();
	ram();
	ram();
	// grab mid top ring
	chassis.pid_odom_set({{70_in, 20_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(500);
	// ram corner
	chassis.pid_odom_set({{6_in, 10_in}, rev, 90});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void blue_7ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(88_in, 12_in, 270_deg);
	// prime actuated intake and grab top ring
	intakeLevel.set(false);
	intakeMove(127);
	ladybrown.move_relative(600, 200);
	chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, 100, 25);
	chassis.pid_wait();
	intakeLevel.set(true);
	// score top ring on alliance stake
	chassis.pid_odom_set(-12.5_in, 80, false);
	chassis.pid_wait();
	pros::delay(500);
	// reset position, back up to not hit wall, and move to + grab mogo
	chassis.odom_xyt_set(72_in, 15_in, 0_deg);
	chassis.pid_odom_set({{{72_in, 30_in}, fwd, 90}, {{96_in, 50_in}, rev, 127}});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	// sweep ring rush rings and score preload on wallstake
	chassis.pid_odom_set({{{100_in, 51_in}, fwd, 90}, {{107_in, 62_in}, fwd, 70}, {{134_in, 63_in}, fwd, 60}});
	chassis.pid_wait();
	ladybrown.move_relative(400, 200);
	pros::delay(750);
	// grab bottom ring of ring stack, then sweep corner
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{134_in, 10_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 90);
	chassis.pid_wait_quick_chain();
	ram();
	ram();
	ram();
	// touch ladder
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

void blue_negsolowp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload
	chassis.pid_turn_set({79_in, 12_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{79_in, 9_in}, fwd, 60});
	chassis.pid_wait();
	ladybrown.move_relative(1000, 200);
	pros::delay(250);
	// grab mogo
	chassis.pid_odom_set({{101_in, 50_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{100_in, 49_in}, fwd, 90}, {{107_in, 61_in}, fwd, 70}, {{134_in, 61_in}, fwd, 60}});
	chassis.pid_wait();
	pros::delay(500);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// grab mid top ring & drop mogo
	chassis.pid_odom_set({{70_in, 22_in}, fwd, 70});
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(650);
	mogomech.set(false);
	pros::delay(100);
	// grab other mogo & grab other bottom ring
	chassis.pid_odom_set({{46_in, 48_in}, rev, 80});
	chassis.pid_wait_quick();
	mogomech.set(true);
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// touch ladder
	chassis.pid_turn_set(90_deg, 90);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(60_in, 90, true);
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

// SKILLS

void skills() {
	chassis.odom_xyt_set(72_in, 12_in, 180_deg);
	// score preload on alliance stake
	ladybrown.move_absolute(1000, 200);
	pros::delay(1000);
	ladybrown.move_absolute(10, 200);
	// grab mogo
	chassis.pid_odom_set({{96_in, 24_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	// score 2 rings on mogo and 1 on wallstake
	chassis.pid_odom_set({{{96_in, 47_in}, fwd, 90}, {{120_in, 48_in}, fwd, 90}, {{130_in, 72_in}, fwd, 90}});
	chassis.pid_wait_until_index(1);
	pros::delay(750);
	ladybrown.move_absolute(200, 200);
	chassis.pid_wait();
	chassis.pid_turn_set(90_deg, 90);
	chassis.pid_wait();
	ladybrown.move_absolute(1000, 200);
	pros::delay(1500);
	ladybrown.move_absolute(10, 200);
	// score 3 more rings on mogo and drop off in corner
	chassis.pid_odom_set({{{120_in, 96_in}, fwd, 90}, {{120_in, 128_in}, fwd, 90}, {{127_in, 131_in}, rev, 60}});
	chassis.pid_wait();
	pros::delay(500);
	mogomech.set(false);
	// score ring on wallstake
	ladybrown.move_absolute(200, 200);
	chassis.pid_odom_set({{{130_in, 118_in}, fwd, 90}, {{130_in, 72_in}, fwd, 90}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, 90);
	chassis.pid_wait();
	ladybrown.move_absolute(1000, 200);
	pros::delay(1500);
	ladybrown.move_absolute(10, 200);
	// grab ring then grab mogo
	chassis.pid_odom_set({{{96_in, 96_in}, fwd, 90}, {{72_in, 120_in}, rev, 80}});
	chassis.pid_wait_until_index(0);
	pros::delay(200);
	intakeMove(0);
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	// score grabbed ring and 5 more on mogo, then drop off
	intakeMove(127);
	chassis.pid_odom_set(
		{{{48_in, 96_in}, fwd, 90}, {{24_in, 96_in}, fwd, 90}, {{24_in, 128_in}, fwd, 90}, {{15_in, 118_in}, fwd, 90}, {{14_in, 126_in}, rev, 90}});
	chassis.pid_wait();
	mogomech.set(false);
	// grab blue ring mogo
	ladybrown.move_absolute(200, 200);
	chassis.pid_odom_set({{44_in, 131_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	// score ring on wallstake, prime another ring for wallstake, then drop off mogo
	chassis.pid_odom_set({{15_in, 72_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(270_deg, 90);
	chassis.pid_wait();
	ladybrown.move_absolute(1000, 200);
	pros::delay(1500);
	ladybrown.move_absolute(200, 200);
	chassis.pid_odom_set({{{15_in, 25_in}, fwd, 90}, {{13_in, 16_in}, rev, 90}});
	chassis.pid_wait();
	mogomech.set(false);
	// score primed ring on wallstake
	chassis.pid_odom_set({{15_in, 72_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(270_deg, 90);
	chassis.pid_wait();
	ladybrown.move_absolute(1000, 200);
	pros::delay(1500);
	ladybrown.move_absolute(10, 200);
	// grab ring then grab mogo
	chassis.pid_odom_set({{{48_in, 48_in}, fwd, 90}, {{48_in, 24_in}, rev, 80}});
	chassis.pid_wait_until_index(0);
	pros::delay(200);
	intakeMove(0);
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	// score grabbed ring and 5 more on mogo, prime ring for ladybrown, then drop off
	intakeMove(127);
	chassis.pid_odom_set(
		{{{24_in, 48_in}, fwd, 90}, {{24_in, 16_in}, fwd, 90}, {{130_in, 24_in}, fwd, 90}, {{122_in, 15_in}, fwd, 90}, {{130_in, 13_in}, rev, 90}});
	chassis.pid_wait_until_index_started(3);
	ladybrown.move_absolute(200, 200);
	chassis.pid_wait();
	mogomech.set(false);
	// score primed ring on wallstake
	chassis.pid_odom_set({{130_in, 72_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, 90);
	chassis.pid_wait();
	ladybrown.move_absolute(1000, 200);
	pros::delay(1500);
	ladybrown.move_absolute(10, 200);
	// grab mid ring and score on alliance stake
	chassis.pid_odom_set({{{96_in, 48_in}, fwd, 90}, {{48_in, 96_in}, fwd, 90}, {{72_in, 132_in}, rev, 90}});
	chassis.pid_wait_until_index_started(1);
	pros::delay(750);
	intakeMove(0);
	chassis.pid_wait();
	chassis.pid_turn_set(180_deg, 90);
	chassis.pid_wait();
	intakeMove(127);
}

void ram() {
	chassis.pid_drive_set(8_in, 127, false);
	chassis.pid_wait();
	chassis.pid_drive_set(-8_in, 127, false);
	chassis.pid_wait();
}
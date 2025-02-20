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
	// same as 5 ring but scoring ONLY on a mogo
	pros::c::task_delete(ringsorting);
}

void red_5ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(88_in, 12_in, 90_deg);
	// same as goal rush wp but without the goal rush, and scoring on alliance stake at the very beginning
	pros::c::task_delete(ringsorting);
}

void red_gr_corner() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(132_in, 20_in, 0_deg);
	// same as goal rush wp but without alliance stake scoring
	pros::c::task_delete(ringsorting);
}

void red_gr_wp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(132_in, 20_in, 0_deg);
	// rush mogo
	doinkerL.set(true);
	chassis.pid_odom_set({{{129_in, 44_in}, fwd, 127}, {{128_in, 58_in}, fwd, 127}});
	chassis.pid_wait_quick();
	doinkerL.set(false);
	chassis.pid_odom_set({{128_in, 27_in}, rev, 90});
	chassis.pid_wait();
	// turn around and regrab mogo with clamp
	doinkerL.set(true);
	pros::delay(200);
	chassis.pid_turn_relative_set(180_deg, 127);
	pros::delay(300);
	doinkerL.set(false);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{126_in, 44_in}, rev, 60});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	pros::delay(200);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{114_in, 52_in}, fwd, 127});
	chassis.pid_wait();
	pros::delay(1000);
	// drop mogo and grab other mogo
	mogomech.set(false);
	chassis.pid_odom_set({{96_in, 48_in}, rev, 90});
	chassis.pid_wait_quick();
	mogomech.set(true);
	// sweep and score corner
	doinkerL.set(true);
	pros::delay(200);
	chassis.pid_odom_set({{{132_in, 9_in}, fwd, 90}, {{120_in, 8_in}, fwd, 60}, {{99_in, 5_in}, fwd, 70}});
	chassis.pid_wait();
	// grab top ring at mid
	chassis.pid_odom_set({{72_in, 12_in}, fwd, 127});
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	doinkerL.set(false);
	// score on alliance stake using ladybrown mounting
	chassis.pid_odom_set({{68_in, 0_in}, fwd, 127});
	chassis.pid_wait_quick();
	ladybrown.move_relative(1000, 200);
	pros::delay(500);
	// touch ladder
	chassis.pid_odom_set({{67_in, 48_in}, rev, 127});
	pros::c::task_delete(ringsorting);
}

void red_6ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	// same as 7 ring but no alliance stake
	pros::c::task_delete(ringsorting);
}

void red_7greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(56_in, 12_in, 270_deg);
	// same as 7 ring but scoring alliance stake thru ladybrown positioning and no wallstake
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
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{10_in, 13_in}, fwd, 90}, {{24_in, 12_in}, fwd, 60}, {{45_in, 9_in}, fwd, 90}});
	chassis.pid_wait_until_index(0);
	doinkerR.set(true);
	chassis.pid_wait();
	// touch ladder
	doinkerR.set(false);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
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
	// same as 5 ring but scoring ONLY on a mogo
	pros::c::task_delete(ringsorting);
}

void blue_5ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(56_in, 12_in, 90_deg);
	// same as goal rush wp but without the goal rush, and scoring on alliance stake at the very beginning
	pros::c::task_delete(ringsorting);
}

void blue_gr_corner() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(12_in, 20_in, 0_deg);
	// same as goal rush wp but without alliance stake scoring
	pros::c::task_delete(ringsorting);
}

void blue_gr_wp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(12_in, 20_in, 0_deg);
	// rush mogo
	doinkerR.set(true);
	chassis.pid_odom_set({{{15_in, 44_in}, fwd, 127}, {{16_in, 58_in}, fwd, 127}});
	chassis.pid_wait_quick();
	doinkerR.set(false);
	chassis.pid_odom_set({{16_in, 27_in}, rev, 90});
	chassis.pid_wait();
	// turn around and regrab mogo with clamp
	doinkerR.set(true);
	pros::delay(200);
	chassis.pid_turn_relative_set(180_deg, 127);
	pros::delay(300);
	doinkerR.set(false);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{24_in, 48_in}, rev, 60});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakeMove(127);
	pros::delay(200);
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{30_in, 52_in}, fwd, 127});
	chassis.pid_wait();
	pros::delay(1000);
	// drop mogo and grab other mogo
	mogomech.set(false);
	chassis.pid_odom_set({{52_in, 40_in}, rev, 90});
	chassis.pid_wait_quick();
	mogomech.set(true);
	// sweep and score corner
	doinkerR.set(true);
	pros::delay(200);
	chassis.pid_odom_set({{{12_in, 9_in}, fwd, 90}, {{24_in, 8_in}, fwd, 60}, {{45_in, 5_in}, fwd, 70}});
	chassis.pid_wait();
	// grab top ring at mid
	chassis.pid_odom_set({{72_in, 12_in}, fwd, 127});
	intakeLevel.set(false);
	chassis.pid_wait();
	intakeLevel.set(true);
	doinkerR.set(false);
	// score on alliance stake using ladybrown mounting
	chassis.pid_odom_set({{76_in, 0_in}, fwd, 127});
	chassis.pid_wait_quick();
	ladybrown.move_relative(1000, 200);
	pros::delay(500);
	// touch ladder
	chassis.pid_odom_set({{77_in, 48_in}, rev, 127});
	pros::c::task_delete(ringsorting);
}

void blue_6ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	// same as 7 ring but no alliance stake
	pros::c::task_delete(ringsorting);
}

void blue_7greed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	chassis.odom_xyt_set(88_in, 12_in, 90_deg);
	// same as 7 ring but scoring alliance stake thru ladybrown positioning and no wallstake
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
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{134_in, 13_in}, fwd, 90}, {{120_in, 12_in}, fwd, 60}, {{99_in, 9_in}, fwd, 90}});
	chassis.pid_wait_until_index(0);
	doinkerL.set(true);
	chassis.pid_wait();
	// touch ladder
	doinkerL.set(false);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
}

// SKILLS

void skills() {}
#include "main.h"  // IWYU pragma: keep

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
	chassis.odom_turn_bias_set(0.9);

	chassis.odom_look_ahead_set(7_in);			 // This is how far ahead in the path the robot looks at
	chassis.odom_boomerang_distance_set(16_in);	 // This sets the maximum distance away from target that the carrot point can be
	chassis.odom_boomerang_dlead_set(0.625);	 // This handles how aggressive the end of boomerang motions are

	chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}

void move_forward() { chassis.pid_drive_set(5_in, DRIVE_SPEED, true); }

// RED

void testautonRed() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	mogomech.set(true);
	intake.move(119);
	chassis.pid_wait();
}

void testautonBlue() {
	pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	mogomech.set(true);
	intake.move(127);
	chassis.pid_wait();
}

void red_gr_wp() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	linedetection = pros::c::task_create(lineDetect, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "line sensing");
	chassis.odom_xyt_set(132_in, 20_in, 0_deg);
	lineTracking = true;
	doinker.set(true);
	chassis.pid_odom_set({{{130_in, 42_in}, fwd, 127}, {{128_in, 56_in, 332_deg}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{128_in, 46_in, 0_deg}, rev, 127});
	ladybrown.move_absolute(1200, 200);
	chassis.pid_wait_quick_chain();
	pros::delay(200);
	doinker.set(false);
	chassis.pid_odom_set({{128_in, 50_in, 180_deg}, rev, 127});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intake.move(127);
	chassis.pid_odom_set({{120_in, 40_in}, fwd, 127});
	chassis.pid_wait();
	pros::delay(500);
	mogomech.set(false);
	chassis.pid_odom_set({{96_in, 48_in, 90_deg}, rev, 90});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakefirst.move(0);
	intake.move(127);
	lineTracking = false;
	chassis.pid_odom_set({{128_in, 16_in, 170_deg}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(290_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{122_in, 12_in, 270_deg}, fwd, 127, ez::ccw}, {{114_in, 18_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	doinker.set(false);
	chassis.pid_odom_set({{72_in, 60_in, -90_deg}, fwd, 127, ez::ccw});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
	pros::c::task_delete(linedetection);
}

void red_7ring() {
	ringsorting = pros::c::task_create(ringsensTask, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	linedetection = pros::c::task_create(lineDetect, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "line sensing");
	chassis.odom_xyt_set(48_in, 20_in, 0_deg);
	lineTracking = false;
	ladybrown.move_absolute(600, 200);
	intakeLevel.set(false);
	chassis.pid_turn_set({72_in, 24_in}, fwd, 90);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	intakefirst.move_relative(600, 200);
	chassis.pid_odom_set({72_in, 12_in, 0_deg, rev, 127});
	chassis.pid_wait_quick();
	intake.move_relative(600, 400);
	pros::delay(500);
	chassis.pid_odom_set({{48_in, 48_in}, rev, 127});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	doinker.set(true);
	intakeLevel.set(true);
	intake.move(127);
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{18_in, 14_in, 260_deg}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(315_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{14_in, 22_in, 0_deg}, fwd, 127, ez::cw}, {{20_in, 28_in}, fwd, 127}, {{16_in, 54_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(45_deg, 90, ez::cw);
	chassis.pid_wait();
	lineTracking = true;
	chassis.pid_odom_set({{{24_in, 65_in}, fwd, 60}, {{36_in, 63_in}, fwd, 90}, {{72_in, 60_in, 90_deg}, fwd, 127}});
	chassis.pid_wait();
	lineTracking = false;
	pros::c::task_delete(ringsorting);
	pros::c::task_delete(linedetection);
}

// BLUE

void blue_gr_wp() {
	pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	linedetection = pros::c::task_create(lineDetect, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "line sensing");
	chassis.odom_xyt_set(12_in, 20_in, 0_deg);
	lineTracking = true;
	doinker.set(true);
	chassis.pid_odom_set({{{14_in, 42_in}, fwd, 127}, {{16_in, 56_in, -62_deg}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{16_in, 46_in, 0_deg}, rev, 127});
	ladybrown.move_absolute(1200, 200);
	chassis.pid_wait_quick_chain();
	pros::delay(200);
	doinker.set(false);
	chassis.pid_odom_set({{16_in, 50_in, 180_deg}, rev, 127});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intake.move(127);
	chassis.pid_odom_set({{24_in, 40_in}, fwd, 127});
	chassis.pid_wait();
	pros::delay(500);
	mogomech.set(false);
	chassis.pid_odom_set({{48_in, 48_in, -90_deg}, rev, 90});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	lineTracking = false;
	chassis.pid_odom_set({{18_in, 14_in, 260_deg}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(315_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{14_in, 22_in, 0_deg}, fwd, 127, ez::cw}, {{20_in, 28_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	doinker.set(false);
	chassis.pid_odom_set({{72_in, 60_in, 90_deg}, fwd, 127, ez::cw});
	chassis.pid_wait();
	pros::c::task_delete(ringsorting);
	pros::c::task_delete(linedetection);
}

void blue_7ring() {
	pros::c::task_create(ringsensTask, (void *)1, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	linedetection = pros::c::task_create(lineDetect, (void *)0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "line sensing");
	chassis.odom_xyt_set(96_in, 20_in, 0_deg);
	lineTracking = false;
	ladybrown.move_absolute(600, 200);
	intakeLevel.set(false);
	chassis.pid_turn_set({72_in, 24_in}, fwd, 90);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	intakefirst.move_relative(600, 200);
	chassis.pid_odom_set({72_in, 12_in, 0_deg, rev, 127});
	chassis.pid_wait_quick();
	intake.move_relative(600, 400);
	pros::delay(500);
	chassis.pid_odom_set({{96_in, 48_in}, rev, 127});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	doinker.set(true);
	intakeLevel.set(true);
	intake.move(127);
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{128_in, 16_in, 170_deg}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(290_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{122_in, 12_in, 270_deg}, fwd, 127, ez::ccw}, {{114_in, 18_in}, fwd, 127}, {{128_in, 54_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(45_deg, 90, ez::ccw);
	chassis.pid_wait();
	lineTracking = true;
	chassis.pid_odom_set({{{120_in, 65_in}, fwd, 60}, {{108_in, 63_in}, fwd, 90}, {{72_in, 60_in, -90_deg}, fwd, 127}});
	chassis.pid_wait();
	lineTracking = false;
	pros::c::task_delete(ringsorting);
	pros::c::task_delete(linedetection);
}

// SKILLS

void skills() {
	chassis.odom_xyt_set(94_in, 4_in, 180_deg);
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{96_in, 36_in, 180_deg}, rev, 127});
	chassis.pid_wait();
	mogomech.set(true);
	intake.move(127);
	chassis.pid_odom_set({{{96_in, 48_in, 0_deg}, fwd, 127},
						  {{130_in, 24_in}, fwd, 90},
						  {{120_in, 14_in}, fwd, 60},
						  {{120_in, 24_in, 315_deg}, fwd, 60},
						  {{128_in, 16_in, 315_deg}, rev, 90}});
	chassis.pid_wait_quick_chain();
	mogomech.set(false);
	ladybrown.move_absolute(180, 200);
	chassis.pid_odom_set({{132_in, 72_in, 90_deg}, fwd, 127});
	chassis.pid_wait_quick_chain();
	intake.move(0);
	ladybrown.move_absolute(1200, 200);
	pros::delay(2000);
	ladybrown.move_absolute(10, 20);
	intakefirst.move(127);
	chassis.pid_odom_set({{{120_in, 96_in}, fwd, 127}, {{98_in, 128_in, 135_deg}, rev, 127}});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intake.move(127);
	ladybrown.move_absolute(1200, 200);
	pros::delay(1000);
	ladybrown.move_absolute(10, 20);
	pros::delay(1000);
	intakefirst.move(0);
	intake.move(127);
	chassis.pid_odom_set(
		{{{116_in, 128_in}, fwd, 90}, {{128_in, 120_in, 135_deg}, fwd, 60}, {{120_in, 120_in, 225_deg}, fwd, 60}, {{128_in, 128_in, 225_deg}, rev, 90}});
	mogomech.set(false);
	intake.move(0);
	chassis.pid_wait();
	intakefirst.move(127);
	chassis.pid_odom_set({{{96_in, 96_in}, fwd, 127}, {{72_in, 120_in, 135_deg}, rev, 127}});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakefirst.move(0);
	intake.move(127);
	chassis.pid_turn_set(225_deg, 90);
	chassis.pid_wait();
	chassis.pid_odom_set({{{48_in, 96_in}, fwd, 127},
						  {{24_in, 96_in, 340_deg}, fwd, 127},
						  {{26_in, 116_in}, fwd, 60},
						  {{24_in, 124_in, 225_deg}, fwd, 60},
						  {{16_in, 116_in, 135_deg}, fwd, 60},
						  {{16_in, 128_in, 135_deg}, rev, 60}});
	chassis.pid_wait();
	mogomech.set(false);
	ladybrown.move_absolute(180, 200);
	chassis.pid_odom_set({{12_in, 72_in, 270_deg}, fwd, 127});
	chassis.pid_wait();
	intake.move(0);
	ladybrown.move_absolute(1200, 200);
	pros::delay(2000);
	ladybrown.move_absolute(10, 20);
	intakefirst.move(127);
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{48_in, 24_in, 300_deg}, rev, 90}});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	intakefirst.move(0);
	intake.move(127);
	chassis.pid_odom_set(
		{{{48_in, 48_in, 240_deg}, fwd, 90}, {{24_in, 24_in, 180_deg}, fwd, 60, ez::ccw}, {{24_in, 16_in}, fwd, 60}, {{12_in, 24_in}, fwd, 60}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(45_deg, 90);
	chassis.pid_wait();
	chassis.pid_odom_set({{16_in, 16_in, 45_deg}, rev, 60});
	chassis.pid_wait();
	mogomech.set(false);
	intake.move(0);
	intakefirst.move(127);
	chassis.pid_odom_set({{72_in, 72_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	intakefirst.move(0);
	intake.move_relative(200, 200);
	chassis.pid_odom_set({{{52_in, 96_in}, fwd, 127}, {{72_in, 132_in, 180_deg}, rev, 127}});
	chassis.pid_wait();
	intake.move(127);
	pros::delay(500);
	intake.move(0);
	ladybrown.move_absolute(1200, 200);
	chassis.pid_odom_set({{{100_in, 100_in}, fwd, 127}, {{72_in, 72_in}, fwd, 127}});
	chassis.pid_wait();
}

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
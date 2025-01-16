#include "main.h"  // IWYU pragma: keep
#include "subsystems.hpp"

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
	printf("test aaaaa");
	pros::Task ringsort(ringsensTask, (void *)"0");
	intake.move(127);
	chassis.pid_wait();
}

void testcolorsortRed() {
	printf("test aaaaa");
	pros::Task ringsort(ringsensTask, (void *)"0");
	intake.move(127);
	chassis.pid_drive_set(60_in, 32, false);
	chassis.pid_wait();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(20_in, 32, false);
	chassis.pid_wait();
	chassis.pid_turn_set(135_deg, TURN_SPEED);
	ladybrown.move_relative(-700, -127);
}

void testautonBlue() {
	printf("test aaaaa");
	pros::Task ringsort(ringsensTask, (void *)"1");
	mogomech.set(true);
	chassis.pid_wait();
}

void testcolorsortBlue() {
	printf("test aaaaa");
	pros::Task ringsort(ringsensTask, (void *)"1");
	intake.move(127);
	chassis.pid_drive_set(60_in, 32, false);
	chassis.pid_wait();
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(20_in, 32, false);
	chassis.pid_wait();
	chassis.pid_turn_set(-135_deg, TURN_SPEED);
	ladybrown.move_relative(-700, -127);
}

void red_gr_wp() {
	pros::Task ringsort(ringsensTask, (void *)0);
	chassis.odom_pose_set({132_in, 21_in, 0_deg});
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
	chassis.pid_odom_set({{128_in, 16_in, 170_deg}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(290_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{122_in, 12_in, 270_deg}, fwd, 127, ez::ccw}, {{114_in, 18_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	doinker.set(false);
	chassis.pid_odom_set({{72_in, 60_in, -90_deg}, fwd, 127, ez::ccw});
	chassis.pid_wait();
}

void red_50WP() {
	pros::Task ringsort(ringsensTask, (void *)0);
	// Get mogo and score 2 rings
	mogomech.set(false);
	chassis.pid_drive_set(-34_in, 75, true);
	chassis.pid_wait_until(-23.5_in);
	mogomech.set(true);
	chassis.pid_wait();
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait();
	intake.move(127);
	chassis.pid_drive_set(22_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	// drop off mogo and touch mid
	pros::delay(2000);
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(48_in, 75, true);
	chassis.pid_wait();
	if(!chassis.interfered) {
		intake.move(0);
		return;
	}
	intake.move(0);
	ladybrown.move_relative(-500, -127);
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

void red_7ring() {
	pros::Task ringsort(ringsensTask, (void *)0);
	chassis.odom_pose_set({54_in, 22_in, 83.6598_deg});
	ladybrown.move_absolute(600, 200);
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	intakefirst.move_relative(600, 200);
	chassis.pid_odom_set({72_in, 12_in, 0_deg, rev, 127});
	chassis.pid_wait_quick();
	intake.move_relative(600, 400);
	pros::delay(500);
	chassis.pid_odom_set({{48_in, 48_in}, rev, 90});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	doinker.set(true);
	intakeLevel.set(false);
	intake.move(127);
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{18_in, 14_in, 260_deg}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(315_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{14_in, 22_in, 0_deg}, fwd, 127, ez::cw}, {{20_in, 28_in}, fwd, 127}, {{16_in, 60_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(45_deg, 90, ez::cw);
	chassis.pid_wait();
	chassis.pid_odom_set({{{24_in, 63_in}, fwd, 60}, {{36_in, 60_in}, fwd, 90}, {{72_in, 60_in, 90_deg}, fwd, 127}});
	chassis.pid_wait();
}

void red_4ring() {
	pros::Task ringsort(ringsensTask, (void *)0);
	// ladybrown.move_relative(-100, -127);
	// score on allaince stake
	chassis.pid_drive_set(-15_in, DRIVE_SPEED, false);
	chassis.pid_wait_until(-8_in);
	chassis.pid_wait();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(-12_in, DRIVE_SPEED, false);
	intake.move(127);
	chassis.pid_wait();
	pros::delay(150);
	chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(225_deg, TURN_SPEED);	// turn to mogo -135
	chassis.pid_wait();
	chassis.pid_drive_set(-34_in, 75, true);  // used to be 35 inches
	chassis.pid_wait_until(-32.75_in);
	mogomech.set(true);
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, TURN_SPEED);  // turn to rings
	chassis.pid_wait();
	chassis.pid_drive_set(28_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_drive_set(-1_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(10_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-12_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(8.75_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-3_in, DRIVE_SPEED, true);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(180_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	ladybrown.move_relative(500, -127);
	chassis.pid_drive_set(50_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

void red_4greed() {
	pros::Task ringsort(ringsensTask, (void *)0);
	// ladybrown.move_relative(-100, -127);
	chassis.pid_drive_set(-30_in, 60, true);
	chassis.pid_wait_until(-27.5_in);
	mogomech.set(true);
	chassis.pid_wait();
	intake.move(127);
	chassis.pid_turn_set(90_deg, TURN_SPEED);  // turn to rings
	chassis.pid_wait();
	chassis.pid_drive_set(27_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_relative_set(95_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(14_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	pros::delay(500);
	chassis.pid_wait();
	chassis.pid_drive_set(-8_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(225_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(14_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_relative_set(45_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	ladybrown.move_relative(-500, -127);
	chassis.pid_wait();
}

void red_6ring() {
	pros::Task ringsort(ringsensTask, (void *)0);
	chassis.pid_drive_set(-30_in, 60, true);
	chassis.pid_wait_until(-27_in);
	mogomech.set(true);
	chassis.pid_wait();
	intake.move(127);
	chassis.pid_turn_set(90_deg, TURN_SPEED);  // turn to rings
	chassis.pid_wait();
	chassis.pid_drive_set(26_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(23_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(34_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(10_in, DRIVE_SPEED, false);
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(10_in, DRIVE_SPEED, false);
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(10_in, DRIVE_SPEED, false);
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(185_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(30_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_drive_set(-8_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(225_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(10_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_relative_set(45_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	ladybrown.move_relative(500, -127);
	chassis.pid_drive_set(30_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

// BLUE

void blue_gr_wp() {
	pros::Task ringsort(ringsensTask, (void *)1);
	chassis.odom_pose_set({12_in, 21_in, 0_deg});
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
	chassis.pid_odom_set({{18_in, 14_in, 260_deg}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(315_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{14_in, 22_in, 0_deg}, fwd, 127, ez::cw}, {{20_in, 28_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	doinker.set(false);
	chassis.pid_odom_set({{72_in, 60_in, 90_deg}, fwd, 127, ez::cw});
	chassis.pid_wait();
}

void blue_50WP() {
	pros::Task ringsort(ringsensTask, (void *)1);
	// Get mogo and score 2 rings
	mogomech.set(false);
	chassis.pid_drive_set(-35_in, 60, true);
	chassis.pid_wait_until(-22.75_in);
	mogomech.set(true);
	chassis.pid_wait();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();
	intake.move(127);
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	// drop off mogo and touch mid
	pros::delay(2000);
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait();
	pros::delay(1500);
	mogomech.set(false);
	intake.move(0);
	chassis.pid_drive_set(48_in, 75, true);
	chassis.pid_wait();
	if(!chassis.interfered) {
		return;
	}
	ladybrown.move_relative(-500, -127);
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

void blue_7ring() {
	pros::Task ringsort(ringsensTask, (void *)1);
	chassis.odom_pose_set({90_in, 22_in, -83.6598_deg});
	ladybrown.move_absolute(600, 200);
	intakeLevel.set(true);
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	intakefirst.move_relative(600, 200);
	chassis.pid_odom_set({72_in, 12_in, 0_deg, rev, 127});
	chassis.pid_wait_quick();
	intake.move_relative(600, 400); 
	pros::delay(500);
	chassis.pid_odom_set({{96_in, 48_in}, rev, 90});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	doinker.set(true);
	intakeLevel.set(false);
	intake.move(127);
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{128_in, 16_in, 170_deg}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(290_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{122_in, 12_in, 270_deg}, fwd, 127, ez::ccw}, {{114_in, 18_in}, fwd, 127}, {{128_in, 60_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(45_deg, 90, ez::ccw);
	chassis.pid_wait();
	chassis.pid_odom_set({{{120_in, 63_in}, fwd, 60}, {{108_in, 60_in}, fwd, 90}, {{72_in, 60_in, -90_deg}, fwd, 127}});
	chassis.pid_wait();
}

void blue_4ring() {
	pros::Task ringsort(ringsensTask, (void *)0);
	// ladybrown.move_relative(-100, -127);
	// score on allaince stake
	chassis.pid_drive_set(-16_in, DRIVE_SPEED, false);
	chassis.pid_wait_until(-8_in);
	chassis.pid_wait();
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait();
	intake.move(127);
	pros::delay(250);
	chassis.pid_drive_set(-12_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	pros::delay(150);
	chassis.pid_drive_set(13_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(-225_deg, TURN_SPEED);	 // turn to mogo 135
	chassis.pid_wait();
	chassis.pid_drive_set(-34_in, 75, true);  // used to be 35 inches
	chassis.pid_wait_until(-32_in);
	mogomech.set(true);
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, 75);  // turn to rings
	chassis.pid_wait();
	chassis.pid_drive_set(27_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(9_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-9_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-7_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(14_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	pros::delay(200);
	chassis.pid_drive_set(-8_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_relative_set(-90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	ladybrown.move_relative(500, -127);
	chassis.pid_drive_set(50_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

void blue_4greed() {
	pros::Task ringsort(ringsensTask, (void *)0);
	// ladybrown.move_relative(-100, -127);
	chassis.pid_drive_set(-30_in, 60, true);
	chassis.pid_wait_until(-27.5_in);
	mogomech.set(true);
	chassis.pid_wait();
	intake.move(127);
	chassis.pid_turn_set(-90_deg, TURN_SPEED);	// turn to rings
	chassis.pid_wait();
	chassis.pid_drive_set(26_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_relative_set(-95_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_wait();
	chassis.pid_drive_set(14_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	pros::delay(500);
	chassis.pid_drive_set(-12, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-225_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(14_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(-8, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_relative_set(-45_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	ladybrown.move_relative(-500, -127);
	chassis.pid_drive_set(48_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

void blue_6ring() {
	pros::Task ringsort(ringsensTask, (void *)0);
	// ladybrown.move_relative(-100, -127);
	chassis.pid_drive_set(-30_in, 75, true);
	chassis.pid_wait_until(-28_in);
	mogomech.set(true);
	chassis.pid_wait();
	intake.move(127);
	chassis.pid_turn_set(-90_deg, TURN_SPEED);	// turn to rings
	chassis.pid_wait();
	chassis.pid_drive_set(26_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(-20_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(40_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(10_in, DRIVE_SPEED, false);
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(10_in, DRIVE_SPEED, false);
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(10_in, DRIVE_SPEED, false);
	pros::delay(500);
	chassis.pid_drive_set(-10_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-190_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(36_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_drive_set(-4_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-225_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(4_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_drive_set(-4, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_relative_set(-45_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	ladybrown.move_relative(500, -127);
	chassis.pid_drive_set(26_in, DRIVE_SPEED, true);
	chassis.pid_wait();
}

// SKILLS

void skills() {
	chassis.pid_drive_set(-2_in, 60, false);
	mogomech.set(true);	 // grab top left mogo
	intake.move(127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-28_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_turn_set(-135_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(66_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(-56_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(43.25_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(153_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(26.75_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	pros::delay(2000);
	mogomech.set(false);
	intake.move(0);
	chassis.pid_turn_set(-64_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-53_in, 75, true);
	chassis.pid_wait_until(-48_in);
	mogomech.set(true);	 // grab center right mogo
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();
	intake.move(127);
	chassis.pid_drive_set(36_in, 100, true);
	chassis.pid_wait();
	chassis.pid_drive_set(-12_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(45_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(34_in, DRIVE_SPEED, true);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(84_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	pros::delay(1000);
	chassis.pid_turn_set(135_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(16.5_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	pros::delay(2000);
	mogomech.set(false);
	chassis.pid_turn_set(180_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(48_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	pros::delay(200);
	intake.move(0);
	chassis.pid_turn_set(-36_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(60_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(-48_in, 75, true);
	chassis.pid_wait_until(-44_in);
	mogomech.set(true);	 // grab bottom left mogo
	chassis.pid_wait();
	intake.move(127);
	chassis.pid_turn_set(180_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(24_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(24_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(20_in, 90, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(10_in, 90, false);
	chassis.pid_wait();
	pros::delay(1000);
	chassis.pid_turn_set(17_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(16.5_in, 90, false);
	chassis.pid_wait();
	chassis.pid_turn_set(180_deg, TURN_SPEED);
	chassis.pid_wait();
	pros::delay(2000);
	mogomech.set(false);
	chassis.pid_drive_set(98_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	pros::delay(200);
	intake.move(0);
	chassis.pid_turn_set(15_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(89.5_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(24_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait();
	intake.move(127);
	pros::delay(250);
	chassis.pid_drive_set(14_in, DRIVE_SPEED, false);
}

/*void skills50() {
	chassis.pid_drive_set(-18_in, 60, false);
	chassis.pid_wait_until(-4_in);
	mogomech.set(true);
	intake.move(127);
	chassis.pid_wait();
	chassis.pid_turn_set(-90_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(27_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	pros::delay(500);
	chassis.pid_turn_set(-180_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(24_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(24_in, DRIVE_SPEED, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(310_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(40_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(113_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(-14_in, 90, false);
	mogomech.set(false);
	intake.move(0);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(135_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(85_in, 90, true);
	chassis.pid_wait_until(75_in);
	intake.move_relative(-2500, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(45_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(34_in, DRIVE_SPEED, true);
	chassis.pid_wait_until(30_in);
	intake.move_relative(-2500, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(180_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(-24_in, 60, false);
	chassis.pid_wait_until(-19_in);
	mogomech.set(true);
	intake.move(127);
	chassis.pid_wait();
	chassis.pid_turn_set(135_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(34_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(27_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	pros::delay(1000);
	chassis.pid_turn_set(105_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(6_in, 40, false);
	chassis.pid_wait();
	pros::delay(1000);
	chassis.pid_turn_set(208_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-8_in, 80, false);
	mogomech.set(false);
	chassis.pid_wait();
	chassis.pid_turn_set(183_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	indexer.set(true);
	intake.move(127);
	chassis.pid_drive_set(62_in, 90, true);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_turn_relative_set(360_deg, 127);
	chassis.pid_wait();
	ladybrown.move_absolute(-700, -90);
	indexer.set(false);
	pros::delay(1000);
	chassis.pid_drive_set(8_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	ladybrown.move_absolute(0, 90);
	chassis.pid_drive_set(-14_in, 75, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(180_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait_until(19_in);
	intake.move_relative(-2500, 127);
	chassis.pid_wait();
	pros::delay(1000);
	chassis.pid_turn_set(270_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
	chassis.pid_wait_until(19_in);
	intake.move_relative(-2500, 127);
	chassis.pid_wait();
	pros::delay(1000);
	chassis.pid_turn_set(45_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-34_in, 60, true);
	chassis.pid_wait_until(30_in);
	mogomech.set(true);
	intake.move_relative(-2500, 127);
	chassis.pid_wait();
	pros::delay(500);
	mogomech.set(false);
	chassis.pid_turn_set(180_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(16_in, 90, false);
	intake.move(127);
	chassis.pid_wait();
	pros::delay(1000);
	chassis.pid_drive_set(-6_in, 60, false);
	chassis.pid_wait();
	chassis.pid_turn_set(95_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(56_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(26_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(12_in, 75, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(267_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_drive_set(114_in, DRIVE_SPEED, true);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(0_deg, TURN_SPEED);
	indexer.set(true);
	intake.move(127);
	chassis.pid_wait();
	chassis.pid_drive_set(60_in, DRIVE_SPEED, true);
	chassis.pid_wait_until(58_in);
	intake.move(0);
	chassis.pid_wait();
	chassis.pid_turn_set(270_deg, TURN_SPEED);
	chassis.pid_wait();
	chassis.pid_turn_relative_set(360_deg, 127);
	chassis.pid_wait();
	ladybrown.move_absolute(-700, -90);
	indexer.set(false);
	pros::delay(1000);
	chassis.pid_drive_set(8_in, DRIVE_SPEED, false);
	chassis.pid_wait();
	ladybrown.move_absolute(0, 90);
	chassis.pid_drive_set(-8_in, 75, false);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(48_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(74_in, DRIVE_SPEED, true);
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, TURN_SPEED);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(16_in, 75, false);
	chassis.pid_wait_until(8_in);
	intake.move(127);
	chassis.pid_wait();
}*/

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
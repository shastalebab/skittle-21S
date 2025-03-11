#include "main.h"  // IWYU pragma: keep

// TODO: Add alliance stake delay, tune all autons, refine brain ui to modify autons easier

/////
// For installation, upgrading, documentations, and tutorials, check out our
// website! https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 127;
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;
const int GRAB_MOGO = 80;

bool cornerState = false;
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

void move_forward() {
	chassis.pid_drive_set(10_in, 127);
	chassis.pid_wait();
}

// RED

void testautonRed() {
	allianceColor = Colors::RED;
	mogoState = AutoMogo::PRIMED;
	setIntake(127);
	chassis.pid_wait();
}

// NEG 4 ring
void red_4pos() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload (with toggle)
	chassis.pid_odom_set({{79.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo
	chassis.pid_drive_set(-42_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-36_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// score top stacked middle ring
	chassis.pid_turn_set(-135_deg, 90);
	chassis.pid_wait_quick();
	intakeLevel.set(false);
	chassis.pid_drive_set(34_in, GRAB_MOGO, true);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(500);
	chassis.pid_drive_set(-34_in, 127, true);
	chassis.pid_wait_quick();
	// score far ring
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 90}, {{123_in, 28_in}, fwd, 90}});
	chassis.pid_wait_quick_chain();
	// score corner ring
	chassis.pid_turn_set(135, 90);
	chassis.pid_wait_quick();
	chassis.drive_set(127, 127);
	pros::delay(2000);
	if(cornerState) {
		// drop goal in corner
		chassis.pid_drive_set(-10_in, 90);
		chassis.pid_wait_quick_chain();
		chassis.pid_turn_set(-45_deg, 90);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-10_in, 90);
		chassis.pid_wait();
		setMogo(false);
	}
	// touch ladder
	chassis.pid_drive_set(cornerState ? 70_in : -70_in, GRAB_MOGO, true);
	setLadyBrown(600);
	chassis.pid_wait();
}

// toggle preload for scoring on allaince or mogo
// NEG 6 ring
void red_6ring() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload
	chassis.pid_odom_set({{64.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo
	chassis.pid_drive_set(-44_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	pros::delay(200);
	// sweep ring rush rings
	chassis.pid_turn_set({35_in, 62_in}, fwd, 60);
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{{35_in, 62_in}, fwd, 90}, {{12_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	pros::delay(250);
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 127, 50, ez::cw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack, then score corner
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 90}, {{21_in, 28_in}, fwd, 90}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-135, 90);
	chassis.pid_wait_quick();
	// ram();
	chassis.drive_set(127, 127);
	pros::delay(2000);
	chassis.pid_drive_set(-22_in, GRAB_MOGO);
	chassis.pid_wait();
	// grab mid top ring
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait();
	chassis.pid_drive_set(52_in, 80);
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait_quick_chain();
	// ram corner
	if(cornerState) {
		chassis.pid_turn_set(272_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
		chassis.pid_turn_set(-65_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	}
	// touch ladder
	chassis.pid_turn_set(0_deg, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(24_in, 127);
}

// NEG Solo WP
void red_negsolowp() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload
	chassis.pid_odom_set({{64.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo
	chassis.pid_drive_set(-42_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-36_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{35_in, 62.5_in}, fwd, 127}, {{11_in, 62.5_in}, fwd, 90}});
	chassis.pid_wait();
	pros::delay(250);
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 127, 50, ez::cw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{{20_in, 48_in}, fwd, 127}, {{66_in, 22_in}, fwd, 90}});
	pros::delay(2000);
	intakeLevel.set(false);
	chassis.pid_wait();
	// grab mid top ring & drop mogo
	pros::delay(500);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(250);
	setIntake(0);
	pros::delay(10);
	intakefirst.move(127);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait();
	pros::delay(500);
	setMogo(false);
	chassis.pid_drive_set(12_in, 127);
	chassis.pid_wait_quick_chain();
	// grab other mogo & grab other bottom ring
	chassis.pid_turn_set(135_deg, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-24_in, 127);
	pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-21_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(24_in, 127);
	chassis.pid_wait();
	// touch ladder
	chassis.pid_turn_set(270_deg, 127);
	chassis.pid_wait_quick_chain();
	setLadyBrown(1000);
	chassis.pid_drive_set(60_in, 127, true);
	chassis.pid_wait();
}

// BLUE

void testautonBlue() {
	allianceColor = Colors::BLUE;
	mogoState = AutoMogo::PRIMED;
	setIntake(127);
	chassis.pid_wait();
}

// POS 4 ring
void blue_4pos() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload (with toggle)
	chassis.pid_odom_set({{64.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo
	chassis.pid_drive_set(-42_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-36_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// score top stacked middle ring
	chassis.pid_turn_set(135_deg, 90);
	chassis.pid_wait_quick();
	intakeLevel.set(false);
	chassis.pid_drive_set(34_in, GRAB_MOGO, true);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(500);
	chassis.pid_drive_set(-34_in, 127, true);
	chassis.pid_wait_quick();
	// score far ring
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 90}, {{21_in, 28_in}, fwd, 90}});
	chassis.pid_wait_quick_chain();
	// score corner ring
	chassis.pid_turn_set(-135, 90);
	chassis.pid_wait_quick();
	chassis.drive_set(127, 127);
	pros::delay(2000);
	if(cornerState) {
		// drop goal in corner
		chassis.pid_drive_set(-10_in, 90);
		chassis.pid_wait_quick_chain();
		chassis.pid_turn_set(45_deg, 90);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-10_in, 90);
		chassis.pid_wait();
		setMogo(false);
	}
	// touch ladder
	chassis.pid_drive_set(cornerState ? 70_in : -70_in, GRAB_MOGO, true);
	setLadyBrown(600);
	chassis.pid_wait();
}
// NEG 6 ring
void blue_6ring() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload
	chassis.pid_odom_set({{79.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo
	chassis.pid_drive_set(-44_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	pros::delay(200);
	// sweep ring rush rings
	chassis.pid_turn_set({109_in, 62_in}, fwd, 60);
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{{109_in, 62_in}, fwd, 90}, {{132_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	pros::delay(250);
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 127, 50, ez::ccw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack, then score corner
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 90}, {{123_in, 28_in}, fwd, 90}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(135, 90);
	chassis.pid_wait_quick();
	// ram();
	chassis.drive_set(127, 127);
	pros::delay(2000);
	chassis.pid_drive_set(-22_in, GRAB_MOGO);
	chassis.pid_wait();
	// grab mid top ring
	chassis.pid_turn_set(-90_deg, 127);
	chassis.pid_wait();
	chassis.pid_drive_set(52_in, 80);
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait_quick_chain();
	// ram corner
	if(cornerState) {
		chassis.pid_turn_set(-272_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
		chassis.pid_turn_set(65_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	}
	// touch ladder
	chassis.pid_turn_set(0_deg, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(24_in, 127);
}

// NEG Solo WP
void blue_negsolowp() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload
	chassis.pid_odom_set({{64.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo
	chassis.pid_drive_set(-42_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-36_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_turn_set({109_in, 62_in}, fwd, 60);
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{{109_in, 62_in}, fwd, 90}, {{132_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	pros::delay(250);
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 127, 50, ez::ccw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{{124_in, 48_in}, fwd, 127}, {{78_in, 22_in}, fwd, 90}});
	pros::delay(2000);
	intakeLevel.set(false);
	chassis.pid_wait();
	// grab mid top ring & drop mogo
	pros::delay(500);
	chassis.pid_wait();
	intakeLevel.set(true);
	pros::delay(250);
	setIntake(0);
	pros::delay(10);
	intakefirst.move(127);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait();
	pros::delay(500);
	setMogo(false);
	chassis.pid_drive_set(12_in, 127);
	chassis.pid_wait_quick_chain();
	// grab other mogo & grab other bottom ring
	chassis.pid_turn_set(225_deg, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-24_in, 127);
	pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-21_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(270_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(24_in, 127);
	chassis.pid_wait();
	// touch ladder
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick_chain();
	setLadyBrown(1000);
	chassis.pid_drive_set(60_in, 127, true);
	chassis.pid_wait();
}

void ram() {
	chassis.drive_set(127, 127);
	pros::delay(1500);
	chassis.pid_drive_set(-16_in, 127);
	chassis.pid_wait_quick();
	intakeLevel.set(false);
	chassis.drive_set(127, 127);
	pros::delay(1000);
	intakeLevel.set(true);
	chassis.pid_drive_set(-16_in, 127);
	chassis.pid_wait_quick_chain();
}
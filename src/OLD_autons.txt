#include "EZ-Template/util.hpp"
#include "autonbuilder.hpp"
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

void red_3safe() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_drive_set(-28_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-22_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(24_in, 127);
	chassis.pid_wait();
	// grab mid top ring
	chassis.pid_turn_set(-116.57_deg, 127);
	chassis.pid_wait();
	chassis.pid_drive_set(53_in, 80);
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait_quick_chain();
	// drop goal in corner
	if(cornerState) {
		chassis.pid_turn_set(88_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
		chassis.pid_turn_set(-65_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	} else {
		// touch ladder
		chassis.pid_turn_set(0_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(24_in, 127);
	}
}

void red_5greed() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_drive_set(-28_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-22_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(24_in, 127);
	chassis.pid_wait();
	// score corner
	chassis.pid_odom_set({{123_in, 24_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(135, fwd, 127);
	chassis.pid_wait_quick();
	ram();
	// grab mid top ring & drop mogo
	chassis.pid_turn_set(-90_deg, 127);
	chassis.pid_wait();
	chassis.pid_drive_set(52_in, 80);
	pros::delay(1000);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait();
	// drop goal in corner
	if(cornerState) {
		chassis.pid_turn_set(88_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
		chassis.pid_turn_set(-65_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	} else {
		// touch ladder
		chassis.pid_turn_set(0_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(24_in, 127);
	}
}

void red_gr_wp() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(107_in, 20_in, 0_deg);
	// rush mogo & collect ring
	setDoinker(Doinker::LEFT, true);
	intakefirst.move(127);
	chassis.pid_odom_set({{120_in, 54_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	setDoinker(Doinker::LEFT, false);
	pros::delay(250);
	chassis.pid_odom_set({{120_in, 28_in}, rev, 90});
	// grab other mogo & score the two collected rings on it
	chassis.pid_odom_set({{96_in, 48_in}, rev, GRAB_MOGO});
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	// drop mogo & regrab goal rush mogo
	pros::delay(1000);
	setMogo(false);
	chassis.pid_odom_set({{120_in, 48_in}, rev, GRAB_MOGO});
	pros::delay(200);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	// score corner
	chassis.pid_odom_set({{134_in, 10_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	ram();
	// grab top ring at mid
	chassis.pid_odom_set({{78_in, 22_in}, fwd, 70});
	pros::delay(1000);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	setIntake(0);
	pros::delay(10);
	intakefirst.move(127);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait();
	setMogo(false);
	// score on alliance stake
	chassis.pid_odom_set({{{72_in, 22_in}, fwd, 90}, {{72_in, 10_in}, rev, 90}});
	chassis.pid_wait();
	setIntake(127);
	pros::delay(500);
	// touch ladder
	setLadyBrown(1000);
	chassis.pid_odom_set({{87_in, 60_in}, rev, 127});
}

void red_possolowp() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload
	chassis.pid_odom_set({{79.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo & bottom ring
	chassis.pid_drive_set(-42_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-36_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(24_in, 127);
	chassis.pid_wait();
	chassis.pid_odom_set({{78_in, 21_in}, fwd, 90});
	pros::delay(2000);
	intakeLevel.set(false);
	chassis.pid_wait();
	// grab mid top ring & drop mogo
	pros::delay(50);
	intakeLevel.set(true);
	pros::delay(240);
	setIntake(0);
	pros::delay(10);
	intakefirst.move(127);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait();
	pros::delay(500);
	setMogo(false);
	chassis.pid_drive_set(12_in, 127);
	chassis.pid_wait_quick_chain();
	// grab mogo
	chassis.pid_turn_set(135_deg, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-24_in, 127);
	pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{35_in, 63.5_in}, fwd, 127}, {{11_in, 63.5_in}, fwd, 90}});
	chassis.pid_wait();
	pros::delay(250);
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 127, 50, ez::cw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// touch ladder
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick_chain();
	setLadyBrown(1000);
	chassis.pid_drive_set(60_in, 127, true);
	chassis.pid_wait();
}

void red_5safe() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_drive_set(-22_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{35_in, 62_in}, fwd, 127}, {{11_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 127, 50, ez::cw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack and mid top ring
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 127});
	chassis.pid_turn_set(116.57_deg, 127);
	chassis.pid_wait();
	chassis.pid_drive_set(53_in, 80);
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait_quick_chain();
	if(cornerState) {
		// ram corner
		chassis.pid_turn_set(-88_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
	} else {
		// touch ladder
		chassis.pid_turn_set(0_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	}
}

void red_6ring() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_drive_set(-22_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{35_in, 62_in}, fwd, 127}, {{11_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 127, 50, ez::cw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack, then score corner
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{21_in, 24_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_turn_set(-135, 90);
	chassis.pid_wait_quick();
	ram();
	// ram corner
	if(cornerState) {
		chassis.pid_turn_set(272_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
	} else {
		// touch ladder
		chassis.pid_turn_set(65_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	}
}

void red_7ring() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload
	chassis.pid_odom_set({{64.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo
	chassis.pid_drive_set(-36_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{35_in, 62.5_in}, fwd, 127}, {{11_in, 62.5_in}, fwd, 90}});
	chassis.pid_wait();
	pros::delay(250);
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 127, 50, ez::cw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack, then score corner
	chassis.pid_odom_set({{{24_in, 48_in}, fwd, 127}, {{21_in, 24_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-135, fwd, 127);
	chassis.pid_wait_quick();
	ram();
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

void blue_3safe() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_drive_set(-28_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-22_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack
	chassis.pid_turn_set(-90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(24_in, 127);
	chassis.pid_wait();
	// grab mid top ring
	chassis.pid_turn_set(116.57_deg, 127);
	chassis.pid_wait();
	chassis.pid_drive_set(53_in, 80);
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait_quick_chain();
	// drop goal in corner
	if(cornerState) {
		chassis.pid_turn_set(-88_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
		chassis.pid_turn_set(0_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	} else {
		// touch ladder
		chassis.pid_turn_set(0_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(24_in, 127);
	}
}

void blue_5greed() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(48_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_drive_set(-28_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-22_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack
	chassis.pid_turn_set(-90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(24_in, 127);
	chassis.pid_wait();
	// score corner
	chassis.pid_odom_set({{21_in, 24_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(225, fwd, 127);
	chassis.pid_wait_quick();
	ram();
	// grab mid top ring & drop mogo
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait();
	chassis.pid_drive_set(52_in, 80);
	pros::delay(1000);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait();
	// drop goal in corner
	if(cornerState) {
		chassis.pid_turn_set(272_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
		chassis.pid_turn_set(65_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	} else {
		// touch ladder
		chassis.pid_turn_set(0_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(24_in, 127);
	}
}

void blue_gr_wp() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(37_in, 20_in, 0_deg);
	// rush mogo & collect ring
	setDoinker(Doinker::RIGHT, true);
	intakefirst.move(127);
	chassis.pid_odom_set({{24_in, 56_in}, fwd, 70});
	chassis.pid_wait_quick_chain();
	setDoinker(Doinker::RIGHT, false);
	pros::delay(250);
	chassis.pid_odom_set({{24_in, 28_in}, rev, 90});
	// grab other mogo & score the two collected rings on it
	chassis.pid_odom_set({{48_in, 48_in}, rev, GRAB_MOGO});
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	// drop mogo & regrab goal rush mogo
	pros::delay(1000);
	setMogo(false);
	chassis.pid_odom_set({{24_in, 48_in}, rev, GRAB_MOGO});
	pros::delay(200);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	// score corner
	chassis.pid_odom_set({{10_in, 10_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	ram();
	// grab top ring at mid
	chassis.pid_odom_set({{66_in, 22_in}, fwd, 70});
	pros::delay(1000);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	setIntake(0);
	pros::delay(10);
	intakefirst.move(127);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait();
	setMogo(false);
	// score on alliance stake
	chassis.pid_odom_set({{{72_in, 22_in}, fwd, 90}, {{72_in, 10_in}, rev, 90}});
	chassis.pid_wait();
	setIntake(127);
	pros::delay(500);
	// touch ladder
	setLadyBrown(1000);
	chassis.pid_odom_set({{57_in, 60_in}, rev, 127});
}

void blue_possolowp() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(60_in, 20_in, 180_deg);
	// score preload
	chassis.pid_odom_set({{64.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo & bottom ring
	chassis.pid_drive_set(-42_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_until(-36_in);
	setMogo(true);
	setIntake(127);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(24_in, 127);
	chassis.pid_wait();
	chassis.pid_odom_set({{66_in, 21_in}, fwd, 90});
	pros::delay(2000);
	intakeLevel.set(false);
	chassis.pid_wait();
	// grab mid top ring & drop mogo
	pros::delay(50);
	intakeLevel.set(true);
	pros::delay(240);
	setIntake(0);
	pros::delay(10);
	intakefirst.move(127);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait();
	pros::delay(500);
	setMogo(false);
	chassis.pid_drive_set(12_in, 127);
	chassis.pid_wait_quick_chain();
	// grab mogo
	chassis.pid_turn_set(225_deg, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-24_in, 127);
	pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{109_in, 63.5_in}, fwd, 127}, {{133_in, 63.5_in}, fwd, 90}});
	chassis.pid_wait();
	pros::delay(250);
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 127, 50, ez::ccw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 127});
	chassis.pid_wait();
	// touch ladder
	chassis.pid_turn_set(270_deg, 127);
	chassis.pid_wait_quick_chain();
	setLadyBrown(1000);
	chassis.pid_drive_set(60_in, 127, true);
	chassis.pid_wait();
}

void blue_5safe() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_drive_set(-22_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{109_in, 62_in}, fwd, 127}, {{133_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 127, 50, ez::ccw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack and mid top ring
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 127});
	chassis.pid_turn_set(-116.57_deg, 127);
	chassis.pid_wait();
	chassis.pid_drive_set(53_in, 80);
	pros::delay(500);
	intakeLevel.set(false);
	chassis.pid_wait();
	pros::delay(500);
	intakeLevel.set(true);
	pros::delay(250);
	chassis.pid_drive_set(-6_in, 90);
	chassis.pid_wait_quick_chain();
	if(cornerState) {
		// ram corner
		chassis.pid_turn_set(88_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
	} else {
		// touch ladder
		chassis.pid_turn_set(0_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	}
}

void blue_6ring() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(96_in, 20_in, 180_deg);
	// grab mogo
	chassis.pid_drive_set(-22_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{109_in, 62_in}, fwd, 127}, {{133_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 127, 50, ez::ccw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack, then score corner
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{123_in, 24_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_turn_set(135, 90);
	chassis.pid_wait_quick();
	ram();
	// ram corner
	if(cornerState) {
		chassis.pid_turn_set(88_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(-100_in, 127);
		chassis.pid_wait();
		setMogo(false);
	} else {
		// touch ladder
		chassis.pid_turn_set(65_deg, 127);
		chassis.pid_wait_quick_chain();
		chassis.pid_drive_set(50_in, 127);
	}
}

void blue_7ring() {
	allianceColor = Colors::BLUE;
	chassis.odom_xyt_set(84_in, 20_in, 180_deg);
	// score preload
	chassis.pid_odom_set({{79.5_in, 9_in}, fwd, 90});
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(500);
	setLadyBrown(10);
	// grab mogo
	chassis.pid_drive_set(-36_in, GRAB_MOGO, true);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	setIntake(127);
	chassis.pid_wait();
	// sweep ring rush rings
	chassis.pid_odom_set({{{109_in, 62.5_in}, fwd, 127}, {{133_in, 62.5_in}, fwd, 90}});
	chassis.pid_wait();
	pros::delay(250);
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 127, 50, ez::ccw);
	chassis.pid_wait_quick_chain();
	// grab bottom ring of ring stack, then score corner
	chassis.pid_odom_set({{{120_in, 48_in}, fwd, 127}, {{123_in, 24_in}, fwd, 127}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(135, fwd, 127);
	chassis.pid_wait_quick();
	ram();
	// grab mid top ring
	chassis.pid_turn_set(270_deg, 127);
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
		chassis.pid_turn_set(88_deg, 127);
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
	chassis.pid_odom_set({{{109_in, 62.5_in}, fwd, 127}, {{133_in, 62.5_in}, fwd, 90}});
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

// SKILLS

void skills() {
	allianceColor = Colors::RED;
	chassis.odom_xyt_set(0_in, 0_in, 90_deg);
	// score preload on alliance stake
	setUnjam(false);
	intake.move_relative(500, 200);
	pros::delay(600);
	setUnjam(true);
	setIntake(127);
	// grab mogo
	chassis.pid_drive_set(15, 127);
	chassis.pid_wait();
	chassis.pid_turn_set(180_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(-18, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(-5, 70);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait();
	pros::delay(250);
	setMogo(true);
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(22, 127);
	chassis.pid_wait_quick();
	chassis.pid_turn_set(0_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(20, 127);
	chassis.pid_wait_quick();
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(59, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(5, 50);
	chassis.pid_wait_quick();
	pros::delay(250);
	chassis.pid_drive_set(-52, 127);
	chassis.pid_wait_quick();
	chassis.pid_turn_set(275_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(54, 100);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(-3, 127);
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(135_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(-10, 127);
	chassis.pid_wait_until(-5);
	setMogo(false);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(6, 127);
	chassis.pid_wait_quick();
	chassis.pid_turn_set(45_deg, 127);
	chassis.pid_wait_quick();
	setUnjam(false);
	setLadyBrown(200);
	chassis.pid_drive_set(6, 127);
	chassis.pid_wait_quick();
	chassis.pid_odom_set({{53, 58}, fwd, 100});
	chassis.pid_wait();
	chassis.pid_turn_set(0, 127);
	chassis.pid_wait_quick();
	intakefirst.move(127);
	setLadyBrown(1200);
	setIntake(0);
	chassis.pid_drive_set(5, 127);
	pros::delay(1000);
	chassis.pid_drive_set(-5, 127);
	setIntake(127);
	setLadyBrown(200);
	pros::delay(1500);
	chassis.pid_drive_set(5, 127);
	setLadyBrown(1200);
	setIntake(0);
	pros::delay(1000);
	setLadyBrown(10);
	chassis.pid_drive_set(-5, 127);
	chassis.pid_wait_quick();
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait_quick();
	chassis.pid_drive_set(48, 127);
	chassis.pid_wait();
	chassis.odom_xyt_set(0, 0, 0);
	chassis.pid_odom_set({{{22_in, -14_in}, fwd, 127}, {{38_in, -6_in}, fwd, 127}, {{-11_in, 15_in}, fwd, 127}});

	/*
	setIntake(127);
	// score 2 rings on mogo & 1 on wallstake
	chassis.pid_odom_set({{{96_in, 47_in}, fwd, 90}, {{120_in, 48_in}, fwd, 90}, {{130_in, 72_in}, fwd, 90}});
	chassis.pid_wait_until_index(1);
	pros::delay(750);
	setLadyBrown(200);
	chassis.pid_wait();
	chassis.pid_turn_set(90_deg, 90);
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(1500);
	setLadyBrown(10);
	// score 3 more rings on mogo & drop off in corner
	chassis.pid_odom_set({{{120_in, 96_in}, fwd, 90}, {{120_in, 128_in}, fwd, 90}, {{127_in, 131_in}, rev, 60}});
	chassis.pid_wait();
	pros::delay(500);
	setMogo(false);
	// score ring on wallstake
	setLadyBrown(200);
	chassis.pid_odom_set({{{130_in, 118_in}, fwd, 90}, {{130_in, 72_in}, fwd, 90}});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, 90);
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(1500);
	setLadyBrown(10);
	// grab ring then grab mogo
	chassis.pid_odom_set({{{96_in, 96_in}, fwd, 90}, {{72_in, 120_in}, rev, GRAB_MOGO}});
	chassis.pid_wait_until_index(0);
	pros::delay(200);
	setIntake(0);
	chassis.pid_wait_quick_chain();
	setMogo(true);
	// score grabbed ring & 5 more on mogo, then drop off
	setIntake(127);
	chassis.pid_odom_set(
		{{{48_in, 96_in}, fwd, 90}, {{24_in, 96_in}, fwd, 90}, {{24_in, 128_in}, fwd, 90}, {{15_in, 118_in}, fwd, 90}, {{14_in, 126_in}, rev, 90}});
	chassis.pid_wait();
	setMogo(false);
	// grab blue ring mogo
	setLadyBrown(200);
	chassis.pid_odom_set({{44_in, 131_in}, rev, GRAB_MOGO});
	chassis.pid_wait_quick_chain();
	setMogo(true);
	// score ring on wallstake, prime another ring for wallstake, then drop off mogo
	chassis.pid_odom_set({{15_in, 72_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(270_deg, 90);
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(1500);
	setLadyBrown(200);
	chassis.pid_odom_set({{{15_in, 25_in}, fwd, 90}, {{13_in, 16_in}, rev, 90}});
	chassis.pid_wait();
	setMogo(false);
	// score primed ring on wallstake
	chassis.pid_odom_set({{15_in, 72_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(270_deg, 90);
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(1500);
	setLadyBrown(10);
	// grab ring then grab mogo
	chassis.pid_odom_set({{{48_in, 48_in}, fwd, 90}, {{48_in, 24_in}, rev, GRAB_MOGO}});
	chassis.pid_wait_until_index(0);
	pros::delay(200);
	setIntake(0);
	chassis.pid_wait_quick_chain();
	setMogo(true);
	// score grabbed ring & 5 more on mogo, prime ring for ladybrown, then drop off
	setIntake(127);
	chassis.pid_odom_set(
		{{{24_in, 48_in}, fwd, 90}, {{24_in, 16_in}, fwd, 90}, {{130_in, 24_in}, fwd, 90}, {{122_in, 15_in}, fwd, 90}, {{130_in, 13_in}, rev, 90}});
	chassis.pid_wait_until_index_started(3);
	setLadyBrown(200);
	chassis.pid_wait();
	setMogo(false);
	// score primed ring on wallstake
	chassis.pid_odom_set({{130_in, 72_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(90_deg, 90);
	chassis.pid_wait();
	setLadyBrown(1000);
	pros::delay(1500);
	setLadyBrown(10);
	// grab mid ring & score on alliance stake
	chassis.pid_odom_set({{{96_in, 48_in}, fwd, 90}, {{48_in, 96_in}, fwd, 90}, {{72_in, 132_in}, rev, 90}});
	chassis.pid_wait_until_index_started(1);
	pros::delay(750);
	setIntake(0);
	chassis.pid_wait();
	chassis.pid_turn_set(180_deg, 90);
	chassis.pid_wait();
	setIntake(127);
	*/
}

void ram() {
	chassis.drive_set(127, 127);
	pros::delay(1500);
	chassis.pid_drive_set(-8_in, 127);
	chassis.pid_wait_quick();
	intakeLevel.set(false);
	chassis.drive_set(127, 127);
	pros::delay(1000);
	intakeLevel.set(true);
	chassis.pid_drive_set(-8_in, 127);
	chassis.pid_wait_quick_chain();
}
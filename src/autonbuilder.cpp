#include "main.h"  // IWYU pragma: keep
#include "subsystems_auton.hpp"

bool red_goal_rush_used = false;
bool blue_goal_rush_used = false;
bool mogo_dropped = false;
Coordinate currentPoint = {0, 0};

void blue_goal_rush() {
	setDoinker(Doinker::RIGHT, true);
	intakefirst.move(127);
	// chassis.pid_odom_set({{24_in, 54_in}, fwd, 90});
	moveToPoint(currentPoint, {24, 54}, 90, fwd);
	chassis.pid_wait_quick_chain();
	setDoinker(Doinker::RIGHT, false);
	pros::delay(250);
	// chassis.pid_odom_set({{24_in, 28_in}, rev, 90});
	moveToPoint(currentPoint, {24, 28}, 90, rev);
	chassis.pid_wait();
	blue_goal_rush_used = true;
	currentPoint = {24, 28};
}

void red_goal_rush() {
	setDoinker(Doinker::LEFT, true);
	intakefirst.move(127);
	// chassis.pid_odom_set({{120_in, 54_in}, fwd, 90});
	moveToPoint(currentPoint, {120, 54}, 90, fwd);
	chassis.pid_wait_quick_chain();
	setDoinker(Doinker::LEFT, false);
	pros::delay(250);
	// chassis.pid_odom_set({{120_in, 28_in}, rev, 90});
	moveToPoint(currentPoint, {120, 28}, 90, rev);
	chassis.pid_wait();
	red_goal_rush_used = true;
	currentPoint = {24, 28};
}

void alliancestake() {
	// chassis.pid_odom_set({{72_in, 16_in}, fwd, 127});
	moveToPoint(currentPoint, {72, 16}, 90, fwd);
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, 70);
	chassis.pid_wait();
	chassis.pid_drive_set(-10, 70);
	chassis.pid_wait();
	intake.move_relative(600, 200);
	pros::delay(200);
	chassis.pid_drive_set(10_in, 90);
	chassis.pid_wait();
	setIntake(127);
	currentPoint = {72, 18};
}

void blue_gr_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 28) {
		// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		moveToPoint(currentPoint, {72, 24}, 127, fwd);
		chassis.pid_wait_quick_chain();
		currentPoint = {72, 24};
	}
	// chassis.pid_odom_set({{24_in, 48_in}, rev, 80});
	moveToPoint(currentPoint, {24, 48}, 80, rev);
	if(mogo_dropped) pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	chassis.pid_wait();
	currentPoint = {24, 48};
}

void red_gr_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 28) {
		// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		moveToPoint(currentPoint, {72, 24}, 127, fwd);
		chassis.pid_wait_quick_chain();
		currentPoint = {72, 24};
	}
	// chassis.pid_odom_set({{120_in, 48_in}, rev, 80});
	moveToPoint(currentPoint, {120, 48}, 80, rev);
	if(mogo_dropped) pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	chassis.pid_wait();
	currentPoint = {120, 48};
}

void left_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 28) {
		// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		moveToPoint(currentPoint, {72, 24}, 127, fwd);
		chassis.pid_wait_quick_chain();
		currentPoint = {72, 24};
	}
	// chassis.pid_odom_set({{48_in, 48_in}, rev, 80});
	moveToPoint(currentPoint, {48, 48}, 80, rev);
	if(mogo_dropped) pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	chassis.pid_wait();
	currentPoint = {48, 48};
}

void right_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 28) {
		// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		moveToPoint(currentPoint, {72, 24}, 127, fwd);
		chassis.pid_wait_quick_chain();
		currentPoint = {72, 24};
	}
	// chassis.pid_odom_set({{96_in, 48_in}, rev, 80});
	moveToPoint(currentPoint, {96, 48}, 80, rev);
	if(mogo_dropped) pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	chassis.pid_wait();
	currentPoint = {96, 48};
}

void left_corner() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 28) {
		// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		moveToPoint(currentPoint, {72, 24}, 127, fwd);
		chassis.pid_wait_quick_chain();
		currentPoint = {72, 24};
	}
	setIntake(127);
	// chassis.pid_odom_set({{21_in, 24_in}, fwd, 127});
	moveToPoint(currentPoint, {21, 24}, 127, fwd);
	chassis.pid_wait_quick();
	chassis.pid_turn_set(-135, 127);
	chassis.pid_wait_quick();
	ram();
	chassis.pid_drive_set(-14_in, 80);
	pros::delay(500);
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait();
	currentPoint = {24, 24};
}

void right_corner() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 28) {
		// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		moveToPoint(currentPoint, {72, 24}, 127, fwd);
		chassis.pid_wait_quick_chain();
		currentPoint = {72, 24};
	}
	setIntake(127);
	// chassis.pid_odom_set({{123_in, 24_in}, fwd, 127});
	moveToPoint(currentPoint, {123, 24}, 127, fwd);
	chassis.pid_wait_quick();
	chassis.pid_turn_set(135, 127);
	chassis.pid_wait_quick();
	ram();
	chassis.pid_drive_set(-14_in, 80);
	pros::delay(500);
	chassis.pid_turn_set(90_deg, 127);
	chassis.pid_wait();
	currentPoint = {120, 24};
}

void red_ring_rush() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 28) {
		// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		moveToPoint(currentPoint, {72, 24}, 127, fwd);
		chassis.pid_wait_quick_chain();
		currentPoint = {72, 24};
	}
	setIntake(127);
	moveToPoint(currentPoint, {48, 48}, 90, fwd);
	chassis.pid_odom_set({{{35_in, 62_in}, fwd, 127}, {{11_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 127, 50, ez::cw);
	chassis.pid_wait_quick_chain();
	currentPoint = {34, 58};
}

void blue_ring_rush() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 28) {
		// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		moveToPoint(currentPoint, {72, 24}, 127, fwd);
		chassis.pid_wait_quick_chain();
		currentPoint = {72, 24};
	}
	setIntake(127);
	moveToPoint(currentPoint, {96, 48}, 90, fwd);
	chassis.pid_odom_set({{{109_in, 62_in}, fwd, 127}, {{133_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 127, 50, ez::ccw);
	chassis.pid_wait_quick_chain();
	currentPoint = {110, 58};
}

void left_btm_ring() {
	setIntake(127);
	if(!blue_goal_rush_used) {
		if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 28) {
			// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
			moveToPoint(currentPoint, {72, 24}, 127, fwd);
			chassis.pid_wait_quick_chain();
			currentPoint = {72, 24};
		}
		// chassis.pid_odom_set({{24_in, 48_in}, fwd, 90});
		moveToPoint(currentPoint, {24, 48}, 127, fwd);
		chassis.pid_wait();
		currentPoint = {24, 48};
	}
}

void mid_top_ring() {
	intakeLevel.set(false);
	intakefirst.move(127);
	// chassis.pid_odom_set({{72_in, 20_in}, fwd, 100});
	moveToPoint(currentPoint, {72, 48}, 90, fwd);
	chassis.pid_wait();
	intakeLevel.set(true);
	currentPoint = {72, 24};
}

void right_btm_ring() {
	setIntake(127);
	if(!red_goal_rush_used) {
		if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 28) {
			// chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
			moveToPoint(currentPoint, {72, 24}, 127, fwd);
			chassis.pid_wait_quick_chain();
			currentPoint = {72, 24};
		}
		// chassis.pid_odom_set({{120_in, 48_in}, fwd, 90});
		moveToPoint(currentPoint, {120, 48}, 127, fwd);
		chassis.pid_wait();
		currentPoint = {120, 48};
	}
}

void ladder() {
	// chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	moveToPoint(currentPoint, {72, 48}, 127, fwd);
	setLadyBrown(600);
	chassis.pid_wait();
	currentPoint = {72, 48};
}

void drop_mogo() {
	setIntake(0);
	setMogo(false);
	mogo_dropped = true;
}
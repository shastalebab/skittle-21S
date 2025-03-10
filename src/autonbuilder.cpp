#include "main.h"  // IWYU pragma: keep

bool red_goal_rush_used = false;
bool blue_goal_rush_used = false;
bool mogo_dropped = false;

void blue_goal_rush() {
	setDoinker(Doinker::RIGHT, true);
	intakefirst.move(127);
	chassis.pid_odom_set({{24_in, 54_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	setDoinker(Doinker::RIGHT, false);
	pros::delay(250);
	chassis.pid_odom_set({{24_in, 28_in}, rev, 90});
	blue_goal_rush_used = true;
}

void red_goal_rush() {
	setDoinker(Doinker::LEFT, true);
	intakefirst.move(127);
	chassis.pid_odom_set({{120_in, 54_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	setDoinker(Doinker::LEFT, false);
	pros::delay(250);
	chassis.pid_odom_set({{120_in, 28_in}, rev, 90});
	red_goal_rush_used = true;
}

void alliancestake() {
	chassis.pid_odom_set({{72_in, 16_in}, fwd, 127});
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
}

void blue_gr_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{24_in, 48_in}, rev, 80});
	if(mogo_dropped) pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	chassis.pid_wait();
}

void red_gr_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{120_in, 48_in}, rev, 80});
	if(mogo_dropped) pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	chassis.pid_wait();
}

void left_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{48_in, 48_in}, rev, 80});
	if(mogo_dropped) pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	chassis.pid_wait();
}

void right_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{96_in, 48_in}, rev, 80});
	if(mogo_dropped) pros::delay(750);
	mogoState = AutoMogo::PRIMED;
	chassis.pid_wait_quick_chain();
	setMogo(true);
	chassis.pid_wait();
}

void left_corner() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	setIntake(127);
	chassis.pid_odom_set({{21_in, 24_in}, fwd, 127});
	chassis.pid_wait_quick();
	chassis.pid_turn_set(-135, 127);
	chassis.pid_wait_quick();
	ram();
	chassis.pid_turn_set({72_in, 24_in}, fwd, 90);
	chassis.pid_wait();
}

void right_corner() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	setIntake(127);
	chassis.pid_odom_set({{123_in, 24_in}, fwd, 127});
	chassis.pid_wait_quick();
	chassis.pid_turn_set(135, 127);
	chassis.pid_wait_quick();
	ram();
	chassis.pid_turn_set({72_in, 24_in}, fwd, 90);
	chassis.pid_wait();
}

void red_ring_rush() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	setIntake(127);
	chassis.pid_odom_set({{{35_in, 62_in}, fwd, 127}, {{11_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 127, 50, ez::cw);
	chassis.pid_wait_quick_chain();
}

void blue_ring_rush() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	setIntake(127);
	chassis.pid_odom_set({{{109_in, 62_in}, fwd, 127}, {{133_in, 62_in}, fwd, 90}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 127, 50, ez::ccw);
	chassis.pid_wait_quick_chain();
}

void left_btm_ring() {
	setIntake(127);
	if(!blue_goal_rush_used) {
		if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
			chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
			chassis.pid_wait_quick_chain();
		}
		chassis.pid_odom_set({{24_in, 48_in}, fwd, 90});
		chassis.pid_wait();
	}
}

void mid_top_ring() {
	intakeLevel.set(false);
	intakefirst.move(127);
	chassis.pid_odom_set({{72_in, 20_in}, fwd, 100});
	chassis.pid_wait();
	intakeLevel.set(true);
}

void right_btm_ring() {
	setIntake(127);
	if(!red_goal_rush_used) {
		if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
			chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
			chassis.pid_wait_quick_chain();
		}
		chassis.pid_odom_set({{120_in, 48_in}, fwd, 90});
		chassis.pid_wait();
	}
}

void ladder() {
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	chassis.pid_wait();
}

void drop_mogo() {
	setIntake(0);
	setMogo(false);
	mogo_dropped = true;
}
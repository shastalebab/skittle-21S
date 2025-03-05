#include "main.h"  // IWYU pragma: keep

bool red_goal_rush_used = false;
bool blue_goal_rush_used = false;

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
	setIntake(0);
	chassis.pid_odom_set({{72_in, 16_in}, fwd, 127});
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, 70);
	chassis.pid_wait();
	chassis.pid_odom_set({{72_in, 10_in}, rev, 70});
	setIntake(127);
}

void blue_gr_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{24_in, 48_in}, rev, 80});
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
	chassis.pid_odom_set({{10_in, 10_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	ram();
	ram();
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
	chassis.pid_odom_set({{134_in, 10_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	ram();
	ram();
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
	chassis.pid_odom_set({{{37_in, 62_in}, fwd, 70}, {{9_in, 62_in}, fwd, 60}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, 90, 50, ez::cw);
	chassis.pid_wait_quick_chain();
}

void blue_ring_rush() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	setIntake(127);
	chassis.pid_odom_set({{{107_in, 62_in}, fwd, 70}, {{135_in, 62_in}, fwd, 60}});
	chassis.pid_wait();
	chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, 90, 50, ez::ccw);
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
	setIntake(127);
	chassis.pid_odom_set({{72_in, 22_in}, fwd, 100});
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
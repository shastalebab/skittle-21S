#include "EZ-Template/util.hpp"
#include "main.h"  // IWYU pragma: keep

bool red_goal_rush_used = false;
bool blue_goal_rush_used = false;

void blue_goal_rush() {
	doinkerR.set(true);
	intakefirst.move(127);
	chassis.pid_odom_set({{24_in, 54_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	doinkerR.set(false);
	pros::delay(250);
	chassis.pid_odom_set({{24_in, 28_in}, rev, 90});
	blue_goal_rush_used = true;
}

void red_goal_rush() {
	doinkerL.set(true);
	intakefirst.move(127);
	chassis.pid_odom_set({{120_in, 54_in}, fwd, 90});
	chassis.pid_wait_quick_chain();
	doinkerL.set(false);
	pros::delay(250);
	chassis.pid_odom_set({{120_in, 28_in}, rev, 90});
	red_goal_rush_used = true;
}

void alliancestake() {
	intakeMove(0);
	chassis.pid_odom_set({{72_in, 16_in}, fwd, 127});
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, 70);
	chassis.pid_wait();
	chassis.pid_odom_set({{72_in, 10_in}, rev, 70});
	intakeMove(127);
}

void blue_gr_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{24_in, 48_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
}

void red_gr_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{120_in, 48_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
}

void left_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{48_in, 48_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
}

void right_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{96_in, 48_in}, rev, 80});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
}

void left_corner() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intakeMove(127);
	chassis.pid_odom_set({{10_in, 10_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set({0_in, 0_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	ram(3);
	chassis.pid_turn_set({72_in, 24_in}, fwd, 90);
	chassis.pid_wait();
}

void right_corner() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intakeMove(127);
	chassis.pid_odom_set({{134_in, 10_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 127);
	chassis.pid_wait_quick_chain();
	ram(3);
	chassis.pid_turn_set({72_in, 24_in}, fwd, 90);
	chassis.pid_wait();
}

void red_ring_rush() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intakeMove(127);
	chassis.pid_odom_set({{{37_in, 62_in}, fwd, 70}, {{9_in, 62_in}, fwd, 60}});
	chassis.pid_wait();
}

void blue_ring_rush() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intakeMove(127);
	chassis.pid_odom_set({{{107_in, 62_in}, fwd, 70}, {{135_in, 62_in}, fwd, 60}});
	chassis.pid_wait();
}

void left_btm_ring() {
	intakeMove(127);
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
	intakeMove(127);
	chassis.pid_odom_set({{72_in, 22_in}, fwd, 100});
	chassis.pid_wait();
	intakeLevel.set(true);
}

void right_btm_ring() {
	intakeMove(127);
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
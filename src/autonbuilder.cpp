#include "EZ-Template/util.hpp"
#include "main.h"  // IWYU pragma: keep

void blue_goal_rush() {
	doinkerR.set(true);
	chassis.pid_odom_set({{{15_in, 44_in}, fwd, 127}, {{16_in, 58_in}, fwd, 60}});
	chassis.pid_wait_quick_chain();
	doinkerR.set(false);
	chassis.pid_odom_set({{16_in, 27_in}, rev, 80});
	chassis.pid_wait();
	doinkerR.set(true);
	pros::delay(200);
	chassis.pid_turn_relative_set(180_deg, 127);
	pros::delay(300);
	doinkerR.set(false);
	chassis.pid_wait();
}

void red_goal_rush() {
	doinkerL.set(true);
	chassis.pid_odom_set({{{129_in, 44_in}, fwd, 127}, {{128_in, 58_in}, fwd, 60}});
	chassis.pid_wait_quick_chain();
	doinkerL.set(false);
	chassis.pid_odom_set({{128_in, 27_in}, rev, 80});
	chassis.pid_wait();
	doinkerL.set(true);
	pros::delay(200);
	chassis.pid_turn_relative_set(180_deg, 127);
	pros::delay(300);
	doinkerL.set(false);
	chassis.pid_wait();
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
	chassis.pid_odom_set({{18_in, 44_in}, rev, 60});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
}

void red_gr_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{126_in, 44_in}, rev, 60});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
}

void left_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{48_in, 48_in}, rev, 100});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
}

void right_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{96_in, 48_in}, rev, 100});
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
	chassis.pid_odom_set({{10_in, 10_in}, fwd, 90});
	chassis.pid_wait();
	chassis.pid_turn_set({0_in, 0_in}, fwd, 90);
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
	intakeMove(127);
	chassis.pid_odom_set({{134_in, 10_in}, fwd, 90});
	chassis.pid_wait();
	chassis.pid_turn_set({144_in, 0_in}, fwd, 90);
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
	intakeMove(127);
	chassis.pid_odom_set({{{44_in, 51_in}, fwd, 90}, {{37_in, 62_in}, fwd, 70}, {{9_in, 62_in}, fwd, 60}});
	chassis.pid_wait();
}

void blue_ring_rush() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intakeMove(127);
	chassis.pid_odom_set({{{100_in, 51_in}, fwd, 90}, {{107_in, 62_in}, fwd, 70}, {{135_in, 62_in}, fwd, 60}});
	chassis.pid_wait();
}

void left_btm_ring() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intakeMove(127);
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 90});
	chassis.pid_wait();
}

void mid_top_ring() {
	intakeLevel.set(false);
	intakeMove(127);
	chassis.pid_odom_set({{72_in, 22_in}, fwd, 100});
	chassis.pid_wait();
	intakeLevel.set(true);
}

void right_btm_ring() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intakeMove(127);
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 90});
	chassis.pid_wait();
}

void ladder() {
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	chassis.pid_wait();
}
#include "EZ-Template/util.hpp"
#include "main.h"  // IWYU pragma: keep

void blue_goal_rush() {
	doinkerL.set(true);
	chassis.pid_odom_set({{{16_in, 56_in}, fwd, 127}, {{16_in, 27_in}, rev, 100}});
	chassis.pid_wait_until_index_started(1);
	doinkerL.set(false);
	chassis.pid_wait();
	chassis.pid_turn_set(180_deg, 127);
	chassis.pid_wait();
}

void red_goal_rush() {
	doinkerR.set(true);
	chassis.pid_odom_set({{{128_in, 56_in}, fwd, 127}, {{128_in, 27_in}, rev, 100}});
	chassis.pid_wait_until_index_started(1);
	doinkerR.set(false);
	chassis.pid_wait();
	chassis.pid_turn_set(180_deg, 127);
	chassis.pid_wait();
}

void alliancestake() {
	intake.move(0);
	chassis.pid_odom_set({{72_in, 16_in}, fwd, 127});
	chassis.pid_wait();
	chassis.pid_turn_set(0_deg, 70);
	chassis.pid_wait();
	chassis.pid_odom_set({{72_in, 10_in}, rev, 70});
	intake.move(119);
}

void blue_gr_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{16_in, 48_in}, rev, 100});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
}

void red_gr_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.pid_odom_set({{128_in, 48_in}, rev, 100});
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
	intake.move(119);
	chassis.pid_odom_set({{14_in, 18_in}, fwd, 90});
	doinkerL.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{48_in, 24_in}, fwd, 60, ez::ccw});
	chassis.pid_wait();
}

void right_corner() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(119);
	chassis.pid_odom_set({{130_in, 18_in}, fwd, 90});
	doinkerL.set(true);
	chassis.pid_wait();
	chassis.pid_odom_set({{96_in, 24_in}, fwd, 60, ez::cw});
	chassis.pid_wait();
}

void red_ring_rush() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(119);
	chassis.pid_odom_set({{{44_in, 50_in}, fwd, 100}, {{37_in, 61_in}, fwd, 80}, {{16_in, 63_in}, fwd, 80}});
	chassis.pid_wait();
}

void blue_ring_rush() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(119);
	chassis.pid_odom_set({{{100_in, 50_in}, fwd, 100}, {{107_in, 61_in}, fwd, 80}, {{128_in, 63_in}, fwd, 80}});
	chassis.pid_wait();
}

void left_btm_ring() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(119);
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 90});
	chassis.pid_wait();
}

void mid_top_ring() {
	intakeLevel.set(false);
	intake.move(119);
	chassis.pid_odom_set({{72_in, 24_in}, fwd, 100});
	chassis.pid_wait();
	intakeLevel.set(true);
}

void right_btm_ring() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(119);
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 90});
	chassis.pid_wait();
}

void ladder() {
	chassis.pid_odom_set({{72_in, 48_in}, fwd, 127});
	chassis.pid_wait();
}
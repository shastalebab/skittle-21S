#include "main.h"  // IWYU pragma: keep

void goal_rush() {
	doinker.set(true);
	chassis.pid_odom_set(62_in, 127, false);
	chassis.pid_wait();
	chassis.pid_odom_set(-24_in, 127, false);
	chassis.pid_wait_quick_chain();
	doinker.set(false);
	chassis.pid_turn_set(180_deg, 127);
	chassis.pid_wait();
}

void alliancestake() {
	intake.move(0);
	chassis.pid_odom_set({{72_in, 24_in, 0_deg}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{72_in, 12_in, 0_deg}, rev, 127});
	chassis.pid_wait_quick_chain();
	intake.move(127);
	chassis.pid_wait();
}

void blue_gr_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_COAST);
	chassis.pid_odom_set({{24_in, 48_in}, rev, 127});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_HOLD);
}

void red_gr_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_COAST);
	chassis.pid_odom_set({{120_in, 48_in}, rev, 127});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_HOLD);
}

void left_mogo() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_COAST);
	chassis.pid_odom_set({{48_in, 48_in}, rev, 127});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_HOLD);
}

void right_mogo() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_COAST);
	chassis.pid_odom_set({{96_in, 48_in}, rev, 127});
	chassis.pid_wait_quick_chain();
	mogomech.set(true);
	chassis.pid_wait();
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_HOLD);
}

void left_corner() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(127);
	chassis.pid_odom_set({{18_in, 14_in, 260_deg}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(315_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{14_in, 22_in, 0_deg}, fwd, 127, ez::cw}, {{20_in, 28_in}, fwd, 127}});
	chassis.pid_wait();
}

void right_corner() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(127);
	chassis.pid_odom_set({{128_in, 16_in, 170_deg}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(290_deg, 127, ez::cw);
	chassis.pid_wait_quick_chain();
	chassis.pid_odom_set({{{122_in, 12_in, 270_deg}, fwd, 127, ez::ccw}, {{114_in, 18_in}, fwd, 127}});
	chassis.pid_wait();
}

void red_ring_rush() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(127);
	chassis.pid_odom_set({{16_in, 54_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(45_deg, 90, ez::cw);
	chassis.pid_wait();
	chassis.pid_odom_set({{{24_in, 65_in}, fwd, 60}, {{36_in, 63_in}, fwd, 90}});
	chassis.pid_wait();
}

void blue_ring_rush() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(127);
	chassis.pid_odom_set({{128_in, 54_in}, fwd, 127});
	chassis.pid_wait_quick_chain();
	chassis.pid_turn_set(-45_deg, 90, ez::ccw);
	chassis.pid_wait();
	chassis.pid_odom_set({{{120_in, 65_in}, fwd, 60}, {{108_in, 63_in}, fwd, 90}});
	chassis.pid_wait();
}

void left_btm_ring() {
	if(chassis.odom_x_get() >= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(127);
	chassis.pid_odom_set({{24_in, 48_in}, fwd, 127});
	chassis.pid_wait();
}

void mid_top_ring() {
	intakeLevel.set(false);
	intake.move(127);
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_COAST);
	chassis.pid_odom_set({{72_in, 28_in}, fwd, 127});
	chassis.pid_wait();
	intakeLevel.set(true);
	chassis.pid_wait();
	chassis.drive_brake_set(pros::E_MOTOR_BRAKE_HOLD);
}

void right_btm_ring() {
	if(chassis.odom_x_get() <= 72 && chassis.odom_y_get() > 36) {
		chassis.pid_odom_set({{72_in, 24_in}, fwd, 127});
		chassis.pid_wait_quick_chain();
	}
	intake.move(127);
	chassis.pid_odom_set({{120_in, 48_in}, fwd, 127});
	chassis.pid_wait();
}

void ladder() {
	chassis.pid_odom_set({{72_in, 60_in}, fwd, 127});
	chassis.pid_wait();
}
#include "main.h"  // IWYU pragma: keep
#include "pros/misc.h"

bool setLB = false;

void setIntake() {
	if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		intake.move(-127);
	else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		intake.move(127);
	else
		intake.move(0);
}

void setLadyBrown() {
	// ladybrown code

	if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
		setLB = !setLB;
		if(ladybrown.get_position() > 280) setLB = false;
		if(setLB) {
			ladybrown.move_absolute(20, 200);
		} else {
			ladybrown.move_absolute(280, 200);
		}
	} else {
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
			ladybrown.move(127);
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
			ladybrown.move(-127);
		else
			ladybrown.move(0);
	}
	/*
	if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
		ladybrown.move(127);
		pros::delay(10);
		ladybrown.move(0);
	}

	if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
		ladybrown.move(-127);
		pros::delay(10);
		ladybrown.move(0);
	}*/
}

void setMogo() { mogomech.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)); }

void setDoinker() { doinker.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)); }

bool shift() { return master.get_digital(pros::E_CONTROLLER_DIGITAL_R1); }
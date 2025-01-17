#include "subsystems.hpp"
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

bool was_preset_pressed = false;
double target = 0.0;
void setLadyBrown() {
	// ladybrown code

	if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
		was_preset_pressed = true;
		setLB = !setLB;
		if(ladybrown.get_position() > 280) setLB = false;
		if(setLB) {
			ladybrown.move_absolute(10, 200);
		} else {
			ladybrown.move_absolute(180, 200);
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
		target = ladybrown.get_position();
		was_preset_pressed = false;
	}

	else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
		ladybrown.move(-127);
		target = ladybrown.get_position();
		was_preset_pressed = false;
	}
	else {
		if (!was_preset_pressed) {
			// ladybrown.move(0);
			ladybrown.move_absolute(target, 20);// this isn't going full power because it was yucky going full power
		}
	}
}

void setMogo() { mogomech.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)); }

void setDoinker() { doinker.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)); }

bool shift() { return master.get_digital(pros::E_CONTROLLER_DIGITAL_R1); }
#include "subsystems.hpp"

#include "main.h"  // IWYU pragma: keep
#include "pros/misc.h"

bool setLB = true;

void setIntake() {
	if(!jammed) {
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
			intakeMove(-127);
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
			intakeMove(127);
		else
			intakeMove(0);
	}
}

bool pressing = false;
void setLadyBrown() {
	// ladybrown code
	if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
		setLB = !setLB;
		pressing = false;
		if(ladybrown.get_position() > 280) setLB = false;
		if(setLB) {
			lbPID.target_set(10);
		} else {
			lbPID.target_set(200);
		}
	} else {
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			pressing = true;
			ladybrown.move(127);
			lbPID.target_set(ladybrown.get_position());
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			pressing = true;
			ladybrown.move(-127);
			lbPID.target_set(ladybrown.get_position());
		} else
			pressing = false;
	}
}

void ladybrownTask() {
	while(true) {
		if(pressing == false) {
			ladybrown.move(lbPID.compute(ladybrown.get_position()));
			pros::delay(10);
		}
	}
}

void setMogo() { mogomech.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)); }

void setDoinkerR() { doinkerR.set(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)); }
void setDoinkerL() { doinkerL.set(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)); }

bool shift() { return master.get_digital(pros::E_CONTROLLER_DIGITAL_R1); }
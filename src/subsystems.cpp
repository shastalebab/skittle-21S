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

void setLadyBrown() {
	// ladybrown code
	if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
		setLB = !setLB;
		if(ladybrown.get_position() > 280) setLB = false;
		if(setLB) {
			lbPID.target_set(10);
		} else {
			lbPID.target_set(200);
		}
	} else {
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			lbPID.target_set(ladybrown.get_position() + 100);
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			lbPID.target_set(ladybrown.get_position() - 100);
		}
	}
}

void ladybrownTask() {
	while(true) {
		ladybrown.move(lbPID.compute(ladybrown.get_position()));
		pros::delay(10);
	}
}

void setMogo() { mogomech.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)); }

void setDoinkerR() { doinkerR.set(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)); }
void setDoinkerL() { doinkerL.set(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)); }

bool shift() { return master.get_digital(pros::E_CONTROLLER_DIGITAL_R1); }
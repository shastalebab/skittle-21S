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

bool was_preset_pressed = false;
double lbtarget = 0.0;
void setLadyBrown() {
	// ladybrown code
	if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
		was_preset_pressed = true;
		setLB = !setLB;
		if(ladybrown.get_position() > 280) setLB = false;
		if(setLB) {
			ladybrown.move_absolute(10, 200);
		} else {
			ladybrown.move_absolute(200, 200);
		}
	} else {
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			ladybrown.move(127);
			lbtarget = ladybrown.get_position();
			was_preset_pressed = false;
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			ladybrown.move(-127);
			lbtarget = ladybrown.get_position();
			was_preset_pressed = false;
		} else {
			if(!was_preset_pressed) {
				ladybrown.move_absolute(lbtarget, 20);	// this isn't going full power because it was yucky going full power
			}
		}
	}
}

void setMogo() { mogomech.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)); }

void setDoinkerR() { doinkerR.set(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)); }
void setDoinkerL() { doinkerL.set(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)); }

bool shift() { return master.get_digital(pros::E_CONTROLLER_DIGITAL_R1); }
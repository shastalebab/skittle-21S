#include "subsystems.hpp"

#include "main.h"  // IWYU pragma: keep
#include "pros/misc.h"

bool setLB = true;

void opcontrolIntake() {
	if(!jammed) {
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
			setIntake(-127);
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
			setIntake(127);
		else
			setIntake(0);
	}
}

void opcontrolLadyBrown() {
	// ladybrown code
	if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
		setLB = !setLB;
		if(ladybrown.get_position() > 280) setLB = false;
		setLadyBrown(setLB ? 10 : 200);
	} else {
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			setLadyBrown(ladybrown.get_position() + 100);
		} else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			setLadyBrown(ladybrown.get_position() - 100);
		}
	}
}

void opcontrolMogo() { mogomech.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)); }

void opcontrolDoinkerR() { doinkerR.set(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)); }
void opcontrolDoinkerL() { doinkerL.set(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)); }

bool shift() { return master.get_digital(pros::E_CONTROLLER_DIGITAL_R1); }
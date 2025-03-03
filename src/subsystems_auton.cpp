#include "subsystems_auton.hpp"

#include "main.h"  // IWYU pragma: keep
#include "subsystems.hpp"

int target = 0;
bool jammed = false;
Colors allianceColor = Colors::NEUTRAL;
AutoMogo mogoState = AutoMogo::OFF;

// Wrappers

void setIntake(int Target) {
	target = Target;
	intake.move(Target);
}

void setLadyBrown(int Target) {
	lbPID.target_set(Target);
}

void setMogo(bool state) {
	mogomech.set(state);
}

void setDoinker(Doinker doinker, bool state) {
	if(doinker == Doinker::LEFT || doinker == Doinker::BOTH) doinkerL.set(state);
	if(doinker == Doinker::RIGHT || doinker == Doinker::BOTH) doinkerR.set(state);
}

// Color sorting

bool discarding = false;

void discard() {
	discarding = true;
	pros::delay(160);
	intakesecond.move(-target);
	pros::delay(100);
	setIntake(target);
	discarding = false;
}

int intakeColor = 2;
int spurfly = 0;
lv_color_t colorList[4] = {lv_color_hex(0xff2a00), lv_color_hex(0x0066cc), lv_color_hex(0x5d5d5d), lv_color_hsv_to_rgb(spurfly, 100, 100)};

void colorSet(Colors color) {
	if(color == Colors::SPUR) {
		spurfly = (spurfly + 1) % 360;	// easter egg
		colorList[3] = lv_color_hsv_to_rgb(spurfly, 100, 100);
	}
	lv_obj_set_style_bg_color(ringind, colorList[(int)color], LV_PART_MAIN);
	lv_obj_set_style_bg_color(mainlabel, colorList[(int)color], LV_PART_MAIN);
	lv_obj_set_style_bg_color(builderringind, colorList[(int)color], LV_PART_MAIN);
}

Colors colorGet() {
	auto hue = ringsens.get_hue();
	if(hue > 0 && hue < 10)
		return Colors::RED;
	else if(hue > 210 && hue < 225)
		return Colors::BLUE;
	else if(hue < 90 && hue > 70)
		return Colors::SPUR;
	else
		return Colors::NEUTRAL;
}

void colorTask() {
	Colors color;
	while(true) {
		color = colorGet();
		ringsens.set_led_pwm(100);
		colorSet(color);
		if(pros::competition::is_autonomous() && discarding == false) {
			if(allianceColor != color && (int)color < 2) {
				discard();
			}
		}
		pros::delay(10);
	}
}

// Other tasks

void ladybrownTask() {
	while(true) {
		ladybrown.move(lbPID.compute(ladybrown.get_position()));
		pros::delay(10);
	}
}

void unjamTask() {
	int jamtime = 0;
	while(intakesecond.get_temperature() < 50) {
		if(setLB == true && discarding == false) {
			if(!jammed && target != 0 && abs(intakesecond.get_actual_velocity()) <= 20) {
				jamtime++;
				if(jamtime > 20) {
					jamtime = 0;
					jammed = true;
				}
			}

			if(jammed) {
				intakesecond.move(-target);
				jamtime++;
				if(jamtime > 20) {
					jamtime = 0;
					jammed = false;
					setIntake(target);
				}
			}
		}
		pros::delay(10);
	}
}

void distanceTask() {
	while(true) {
		if(mogoState == AutoMogo::PRIMED) {
			if(distsens.get() < 300) mogomech.set(true);
			mogoState = AutoMogo::OFF;
		}
		pros::delay(10);
	}
}
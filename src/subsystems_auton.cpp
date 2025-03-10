#include "main.h"  // IWYU pragma: keep
#include "pros/misc.hpp"
#include "subsystems.hpp"

int target = 0;
bool unjam = true;
bool jammed = false;
bool taring = true;
bool usingTarget = true;
Colors allianceColor = Colors::NEUTRAL;
AutoMogo mogoState = AutoMogo::OFF;

// Wrappers

void setIntake(int Target) {
	target = Target;
	intake.move(Target);
}

void setLadyBrown(int Target) { lbPID.target_set(Target); }

void tareLadyBrown() {
	taring = true;
}

void setMogo(bool state) { mogomech.set(state); }

void setDoinker(Doinker doinker, bool state) {
	if(doinker == Doinker::LEFT || doinker == Doinker::BOTH) doinkerL.set(state);
	if(doinker == Doinker::RIGHT || doinker == Doinker::BOTH) doinkerR.set(state);
}

void setUnjam(bool state) { unjam = state; }

// Color sorting

bool discarding = false;

void discard() {
	discarding = true;
	setLadyBrown(100);
	pros::delay(300);
	tareLadyBrown();
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
	if(hue > 0 && hue < 15)
		return Colors::RED;
	else if(hue > 200 && hue < 215)
		return Colors::BLUE;
	else if(hue < 90 && hue > 70)
		return Colors::SPUR;
	else
		return Colors::NEUTRAL;
}

bool colorCompare(Colors color) {
	if((int)allianceColor < 2 && (int)color < 2) return allianceColor != color;
	return false;
}

void colorTask() {
	Colors color;
	ringsens.set_integration_time(10);
	ringsens.set_led_pwm(100);
	while(true) {
		color = colorGet();
		colorSet(color);
		if(!jammed && pros::competition::is_autonomous()) {
			if(colorCompare(color) && !discarding) {
				discard();
			}
		}
	}
	pros::delay(10);
}

// Other tasks

void ladybrownTask() {
	while(true) {
		if(taring) {
			ladybrown.move(-127);
			if(abs(ladybrown.get_actual_velocity()) < 20) {
				ladybrown.tare_position();
				setLadyBrown(10);
				taring = false;
			}
		} else if(usingTarget) ladybrown.move(lbPID.compute(ladybrown.get_position()));
		pros::delay(10);
	}
}

void unjamTask() {
	int jamtime = 0;
	while(true) {
		if(intakesecond.get_temperature() < 50 && unjam) {
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
		}
		pros::delay(10);
	}
}

void distanceTask() {
	while(true) {
		if(pros::competition::is_autonomous() && mogoState == AutoMogo::PRIMED) {
			if(distsens.get() < 40) {
				mogomech.set(true);
				mogoState = AutoMogo::OFF;
			}
		}
		pros::delay(10);
	}
}
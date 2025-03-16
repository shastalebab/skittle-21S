#include "subsystems_auton.hpp"

#include "main.h"  // IWYU pragma: keep
#include "pros/misc.hpp"
#include "pros/rtos.hpp"


int target = 0;
bool unjam = true;
bool jammed = false;
bool taring = false;
bool usingTarget = true;
Colors allianceColor = Colors::NEUTRAL;
AutoMogo mogoState = AutoMogo::OFF;

// Wrappers

void setIntake(int Target) {
	target = Target;
	intake.move(Target);
}

void setLadyBrown(int Target) { lbPID.target_set(Target); }

void tareLadyBrown() { taring = true; }

void setMogo(bool state) { mogomech.set(state); }

void setDoinker(Doinker doinker, bool state) {
	if(doinker == Doinker::LEFT || doinker == Doinker::BOTH) doinkerL.set(state);
	if(doinker == Doinker::RIGHT || doinker == Doinker::BOTH) doinkerR.set(state);
}

void setUnjam(bool state) { unjam = state; }

// Color sorting

bool discarding = false;

void discard() {
	intakesecond.move(-127);
	pros::delay(80);
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
	if(ringsens.get_proximity() > 100) {
		if((hue > 340 && hue < 360) || (hue > 0 && hue < 15))
			return Colors::RED;
		else if(hue > 210 && hue < 225)
			return Colors::BLUE;
		else if(hue < 90 && hue > 70)
			return Colors::SPUR;
	} return Colors::NEUTRAL;
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
		if(!jammed && pros::competition::is_autonomous() && !pros::competition::is_disabled()) {
			if(colorCompare(color) && !discarding) {
				discarding = true;
			} else if(discarding) {
				if(hooksens.get_value() < 2800 && util::sgn(intakesecond.get_actual_velocity()) == 1) discard();
			}
		}
		pros::delay(10);
	}
}

// Other tasks

void ladybrownTask() {
	int taretime = 0;
	while(true) {
		if(taring) {
			ladybrown.move(-127);
			if(abs(ladybrown.get_actual_velocity()) < 5) taretime++;
			if(taretime > 10) {
				ladybrown.move(0);
				pros::delay(10);
				ladybrown.set_zero_position(-60);
				setLadyBrown(10);
				taretime = 0;
				taring = false;
			}
		} else if(usingTarget)
			ladybrown.move(lbPID.compute(ladybrown.get_position()));
		pros::delay(10);
	}
}

void unjamTask() {
	int jamtime = 0;
	while(true) {
		if(intakesecond.get_temperature() < 50 && unjam) {
			if(setLB) {
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

// LEDs

int compMode = 0;
int compStatus() { return compMode; }

int comp1 = 0;
int compPluged() { return comp1; }
void compStatusSet(int mode, int set) {
	compMode = mode;
	if(set <= 1) {
		comp1 = set;
	}
}

void ledTimeTask() {
	int i = 0;
	bool match = true;
	while(match) {
		if(compStatus() == 2 && compPluged() == 1) {
			LEDmanager.setColor(WHITE_HEX);
			driverClock.waitUntil(40 * 1000);
			while(i < 10) {
				LEDmanager.setColor(SPURFLY_HEX);
				pros::delay(500);
				LEDmanager.setColor(WHITE_HEX);
				pros::delay(500);
				master.rumble(".");
				i++;
			}
			driverClock.waitUntil(30 * 1000);
			master.rumble(".....");
			LEDmanager.off();
			LEDmanager.rainbow(0);
			driverClock.waitUntil(0);
			LEDmanager.off();
			match = false;
		}
		pros::delay(20);
	}
}

void ledAllianceTask() {
	int i = 0;
	while(i < 1) {
		if(compStatus() == 0 && compPluged() == 1) {
			switch(allianceColor) {
				case Colors::RED:
					LEDmanager.setColor(RED_HEX);
					break;
				case Colors::BLUE:
					LEDmanager.setColor(BLUE_HEX);
					break;
				default:
					LEDmanager.setColor(RED_HEX);
			}
		} else if(compPluged() == 0) {
			LEDmanager.setColor(WHITE_HEX);
		} else if(compStatus() >= 1) {
			LEDmanager.off();
			i++;
			break;
		}
		pros::delay(20);
	}
}

// Auton pathing aids

okapi::QLength findDistance(Coordinate point1, Coordinate point2, ez::drive_directions direction) {
	auto new_direction = direction == rev ? -1 : 1;
	double errorX = point2.x - point1.x;
	double errorY = point2.y - point1.y;
	return ((sqrt((errorX * errorX) + (errorY * errorY))) * new_direction) * okapi::inch;
}

okapi::QAngle findAngle(Coordinate point1, Coordinate point2, ez::drive_directions direction) {
	auto new_direction = direction == rev ? 180 : 0;
	double errorX = point2.x - point1.x;
	double errorY = point2.y - point1.y;
	return ((atan2(errorX, errorY) * 180 / M_PI) + new_direction) * okapi::degree;
}

void moveToPoint(Coordinate currentpoint, Coordinate newpoint, int speed, ez::drive_directions direction) {
	bool slew_state = false;
	if(findDistance(currentpoint, newpoint, direction) > 24_in && speed > 90) slew_state = true;
	chassis.pid_turn_set(findAngle(currentpoint, newpoint, direction), speed);
	chassis.pid_wait_quick_chain();
	chassis.pid_drive_set(findDistance(currentpoint, newpoint, direction), speed, slew_state);
}
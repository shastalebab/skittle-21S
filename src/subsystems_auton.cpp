#include "main.h"  // IWYU pragma: keep
#include "subsystems.hpp"

int intakeColor = 2;
int spurfly;
typedef struct alliancecolor {
	int alliance;
} alliancecolor;
int target = 0;
bool lineTracking = false;
bool jammed = false;

void intakeMove(int Target) {
	target = Target;
	intake.move(Target);
}

void colorDetect() {
	while(true) {
		ringsens.set_led_pwm(100);
		if((ringsens.get_hue() < 10) && (ringsens.get_hue() > 0)) {
			intakeColor = 1;  // red
			lv_obj_set_style_bg_color(ringind, lv_color_hex(0xff2a00), LV_PART_MAIN);
			lv_obj_set_style_bg_color(mainlabel, lv_color_hex(0xff2a00), LV_PART_MAIN);
			lv_obj_set_style_bg_color(builderringind, lv_color_hex(0xff2a00), LV_PART_MAIN);
		} else if((ringsens.get_hue() < 225) && (ringsens.get_hue() > 210)) {
			intakeColor = 0;  // blue
			lv_obj_set_style_bg_color(ringind, lv_color_hex(0x0066cc), LV_PART_MAIN);
			lv_obj_set_style_bg_color(mainlabel, lv_color_hex(0x0066cc), LV_PART_MAIN);
			lv_obj_set_style_bg_color(builderringind, lv_color_hex(0x0066cc), LV_PART_MAIN);
		} else if((ringsens.get_hue() < 90) && (ringsens.get_hue() > 70)) {
			spurfly = (spurfly + 1) % 360;	// easter egg
			lv_obj_set_style_bg_color(ringind, lv_color_hsv_to_rgb(spurfly, 100, 100), LV_PART_MAIN);
			lv_obj_set_style_bg_color(mainlabel, lv_color_hsv_to_rgb(spurfly, 100, 100), LV_PART_MAIN);
			lv_obj_set_style_bg_color(builderringind, lv_color_hsv_to_rgb(spurfly, 100, 100), LV_PART_MAIN);
		} else {
			intakeColor = 2;  // neutral
			lv_obj_set_style_bg_color(ringind, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
			lv_obj_set_style_bg_color(mainlabel, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
			lv_obj_set_style_bg_color(builderringind, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
		}
		pros::delay(10);
	}
}

bool discarding = false;

void discard() {
	discarding = true;
	pros::delay(160);
	intakesecond.move(-target);
	pros::delay(100);
	intakeMove(target);
	discarding = false;
}

void ringsensTask(void* assign) {
	while(true) {
		alliancecolor allianceColor;
		allianceColor.alliance = int((int*)assign);
		if(allianceColor.alliance == intakeColor && discarding == false) discard();
		pros::delay(10);
	}
}

void unjamTask() {
	int jamtime = 0;
	bool jamfirst = false;
	while(setLB == true && discarding == false && intakesecond.get_temperature() < 50) {
		if(!jammed && target != 0 && (abs(intakefirst.get_actual_velocity()) <= 20 || abs(intakesecond.get_actual_velocity()) <= 20)) {
			jamtime++;
			if(abs(intakefirst.get_actual_velocity()) <= 20)
				jamfirst = true;
			else
				jamfirst = false;
			if(jamtime > 20) {
				jamtime = 0;
				jammed = true;
			}
		}

		if(jammed) {
			(jamfirst ? intakefirst : intakesecond).move(-target);
			jamtime++;
			if(jamtime > 20) {
				jamtime = 0;
				jammed = false;
				jamfirst = false;
				intakeMove(target);
			}
		}
		pros::delay(10);
	}
}

/*void lineDetect(void* test) {
	while(lineTracking) {
		bool linedetect = linesafety.get_value() < 2000 ? true : false;
		if(linedetect) chassis.odom_xyt_set(chassis.odom_x_get() * okapi::inch, 72_in, chassis.odom_theta_get() * okapi::degree);
	}
}*/
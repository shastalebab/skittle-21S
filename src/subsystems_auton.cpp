#include "main.h"  // IWYU pragma: keep

int intakeColor = 2;
int spurfly;
typedef struct alliancecolor {
	int alliance;
} alliancecolor;
int target = 0;
bool lineTracking = false;
bool jammed = false;

void intakeMove(int Target) {
	intake.move(Target);
	target = Target;
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
	intake.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	intake.move(0);
	pros::delay(50);
	int discTarget = (int)intake.get_position() % 1000;
	intake.move_relative(1000 - discTarget, 200);
	pros::delay(500);
	intake.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
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
	while(setLB) {
		if(!jammed && target != 0 && abs(intakesecond.get_actual_velocity()) <= 20) {
			jamtime++;
			if(jamtime > 25) {
				jamtime = 0;
				jammed = true;
			}
		} 

		if(jammed) {
			intakesecond.move(-target);
			jamtime++;
			if(jamtime > 25) {
				jamtime = 0;
				jammed = false;
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
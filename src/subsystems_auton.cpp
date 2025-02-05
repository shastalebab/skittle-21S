#include "main.h"  // IWYU pragma: keep

int intakeColor = 2;
int spurfly;
typedef struct alliancecolor {
	int alliance;
} alliancecolor;
bool lineTracking = false;

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
	double voltage = intake.get_voltage();
	intake.move(0);
	pros::delay(50);
	int target = (int)intake.get_position()%1000;
	intake.move_relative(1000-target, 200);
	pros::delay(500);
	intake.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
	intake.move(abs(voltage));
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

void lineDetect(void* test) {
	while(lineTracking) {
		bool linedetect = linesafety.get_value() < 2000 ? true : false;
		if(linedetect) chassis.odom_xyt_set(chassis.odom_x_get() * okapi::inch, 72_in, chassis.odom_theta_get() * okapi::degree);
	}
}
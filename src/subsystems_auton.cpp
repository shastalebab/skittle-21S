#include "main.h"  // IWYU pragma: keep

int intakeColor = 2;
int spurfly;
typedef struct alliancecolor {
	int alliance;
} alliancecolor;

void colorDetect() {
	while(true) {
		ringsens.set_led_pwm(50);
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
	pros::delay(75);
	intake.move(0);
	pros::delay(300);
	intake.move(127);
	discarding = false;
}

// this function is wrong, fix it
void ringsensTask(void* assign) {
	while(true) {
		alliancecolor allianceColor;
		allianceColor.alliance = int((int*)assign);
		cout << allianceColor.alliance << intakeColor << endl;
		if(allianceColor.alliance == intakeColor && discarding == false) discard();
		pros::delay(10);
	}
}
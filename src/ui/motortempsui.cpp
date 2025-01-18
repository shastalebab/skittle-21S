#include "main.h"  // IWYU pragma: keep
// TODO: refactor this to automatically detect motors instead of manually setting them (using pros::c::get_plugged_type(i) == 2)
float temp = 0;
float driveTemp;
float intakeTemp;

vector<lv_obj_t *> motorboxes{};
vector<int> motorindex{};

lv_obj_t *motorview;
lv_obj_t *motorinfo;
lv_obj_t *motorbackground = lv_img_create(motortemps);
lv_obj_t *mainlabel = lv_label_create(motortemps);
static lv_style_t stylemotor;

LV_IMG_DECLARE(brainbg);

static void gettemp(lv_event_t *e) {
	const char *getmotor = (char *)lv_event_get_user_data(e);
	temp = (motorbar[*getmotor].motor.get_temperature() - 30) * 5;
	lv_obj_set_style_bg_opa(lv_event_get_target(e), temp, LV_PART_MAIN);
}

static void tempmore(lv_event_t *e) {
	const char *getinfo = (char *)lv_event_get_user_data(e);
	motorinfo = lv_msgbox_create(NULL, (motorbar[*getinfo].name + " temperature:").c_str(),
								 (std::to_string(motorbar[*getinfo].motor.get_temperature(1)) + "°C").c_str(), NULL, true);
	lv_obj_add_style(lv_msgbox_get_close_btn(motorinfo), &stylemotor, LV_PART_MAIN);
	lv_obj_add_style(motorinfo, &stylemotor, LV_PART_MAIN);
	lv_obj_set_style_text_font(motorinfo, &lv_font_montserrat_48, LV_PART_MAIN);
	lv_obj_set_style_text_font(lv_msgbox_get_title(motorinfo), &lv_font_montserrat_14, LV_PART_MAIN);
	lv_obj_set_style_text_color(motorinfo, lv_color_hex(0xcfffe9), LV_PART_MAIN);
	lv_obj_set_style_text_font(lv_msgbox_get_close_btn(motorinfo), &lv_font_montserrat_24, LV_PART_MAIN);
	lv_obj_set_style_text_color(lv_msgbox_get_close_btn(motorinfo), lv_color_hex(0xcfffe9), LV_PART_MAIN);
	lv_obj_set_style_border_opa(lv_msgbox_get_close_btn(motorinfo), 0, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(motorinfo, 255, LV_PART_MAIN);
	lv_obj_align(motorinfo, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_bg_color(motorinfo, lv_color_hsv_to_rgb(124, 71, temp), LV_PART_MAIN);
}

lv_event_cb_t getTemp = gettemp;
lv_event_cb_t tempMore = tempmore;

void tempcheck() {
	lv_style_init(&stylemotor);
	lv_style_set_border_color(&stylemotor, lv_color_hex(0xcfffe9));
	lv_style_set_border_width(&stylemotor, 2);
	lv_style_set_radius(&stylemotor, 8);
	lv_style_set_text_color(&stylemotor, lv_color_hex(0x071808));
	lv_style_set_bg_color(&stylemotor, lv_color_hex(0xcfffe9));
	lv_style_set_bg_opa(&stylemotor, 0);
	lv_style_set_text_font(&stylemotor, &pros_font_dejavu_mono_18);
	lv_style_set_text_align(&stylemotor, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_ver(&stylemotor, 6);
	int motorrow = 0;
	char vectorprobe = 0;
	for(int j = 0; j < motorbar.size(); j++) motorindex.push_back(j);
	for(int b = 0; b < motorbar.size(); b++) {
		motorview = lv_label_create(motortemps);
		lv_obj_add_style(motorview, &stylemotor, LV_PART_MAIN);
		lv_obj_add_flag(motorview, LV_OBJ_FLAG_CLICKABLE);
		lv_obj_set_size(motorview, 97, 39);
		lv_label_set_long_mode(motorview, LV_LABEL_LONG_SCROLL);
		lv_label_set_text_fmt(motorview, "%s", motorbar[b].name.c_str());
		lv_obj_set_style_transform_width(motorview, 2, LV_STATE_PRESSED);
		lv_obj_set_style_transform_height(motorview, 2, LV_STATE_PRESSED);
		lv_obj_add_event_cb(motorview, getTemp, LV_EVENT_REFRESH, &motorindex[b]);
		lv_obj_add_event_cb(motorview, tempMore, LV_EVENT_CLICKED, &motorindex[b]);
		if(b != 0 && b % 4 == 0) motorrow++;
		lv_obj_set_pos(motorview, (17 + b % 4 * 116), (68 + motorrow * 58));
		motorboxes.push_back(motorview);
	}

	lv_img_set_src(motorbackground, &brainbg);	//"S:brainbg.bin"
	lv_obj_move_background(motorbackground);

	lv_obj_add_style(mainlabel, &stylemotor, LV_PART_MAIN);
	lv_obj_set_style_pad_all(mainlabel, 6, LV_PART_MAIN);
	lv_obj_set_style_text_color(mainlabel, lv_color_hex(0xcfffe9), LV_PART_MAIN);
	lv_obj_set_style_text_font(mainlabel, &lv_font_montserrat_36, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(mainlabel, 255, LV_PART_MAIN);
	lv_obj_align(mainlabel, LV_ALIGN_TOP_MID, 0, 6);
	lv_label_set_text(mainlabel, "Motor Temperatures");
}

void tempcheckctrl() {
	while(true) {
		driveTemp = (motorbar[1].motor.get_temperature() + motorbar[2].motor.get_temperature() + motorbar[3].motor.get_temperature() +
					 motorbar[5].motor.get_temperature() + motorbar[6].motor.get_temperature() + motorbar[7].motor.get_temperature()) /
					6;
		intakeTemp = (motorbar[0].motor.get_temperature() + motorbar[4].motor.get_temperature()) / 2;

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) == true) {
			if(intakeTemp <= 30)
				pros::c::controller_print(pros::E_CONTROLLER_MASTER, 0, 0, "i: cool, %.0f°C    ", intakeTemp);
			else if(intakeTemp > 30 && intakeTemp <= 45)
				pros::c::controller_print(pros::E_CONTROLLER_MASTER, 0, 0, "i: warm, %.0f°C    ", intakeTemp);
			else if(intakeTemp > 45)
				pros::c::controller_print(pros::E_CONTROLLER_MASTER, 0, 0, "i: hot, %.0f°C    ", intakeTemp);
			pros::delay(90);
		} else {
			if(driveTemp <= 30)
				pros::c::controller_print(pros::E_CONTROLLER_MASTER, 0, 0, "d: cool, %.0f°C    ", driveTemp);
			else if(driveTemp > 30 && driveTemp <= 45)
				pros::c::controller_print(pros::E_CONTROLLER_MASTER, 0, 0, "d: warm, %.0f°C    ", driveTemp);
			else if(driveTemp > 45)
				pros::c::controller_print(pros::E_CONTROLLER_MASTER, 0, 0, "d: hot, %.0f°C    ", driveTemp);
			pros::delay(90);
		}
		pros::delay(10);
	}
}
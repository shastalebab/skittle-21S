#include "main.h"  // IWYU pragma: keep

// TO DO: Add callbacks. Make it so it goes back a page when you remove all
// modules on a page.
jas::autobuildermodules::autobuildermodules() {
	Name = "";
	Desc = "";
	Points = {};
}
jas::autobuildermodules::autobuildermodules(std::string name, std::string desc) {
	Name = name;
	Desc = desc;
	Points = {};
}
jas::autobuildermodules::autobuildermodules(std::string name, std::string desc, vector<pair<int, int>> points) {
	Name = name;
	Desc = desc;
	Points = points;
}

int l = 0;
int k = 0;
int selected1 = 0;
int selected2 = 0;
int color = 1;
int startposition = 0;
bool activescreen = false;
bool positionorient = true;
bool noselection1 = true;
bool noselection2 = true;
pros::task_t ringsorting;

vector<jas::autobuildermodules> getmodules{
	jas::autobuildermodules("goal rush", "Use doinker to grab mogo at the intersection between alliances"),
	jas::autobuildermodules("alliance stake", "Score on the alliance stake"),
	jas::autobuildermodules("blue regrab goal rush", "Grab the goal rush mogo from where it was first dropped off"),
	jas::autobuildermodules("red regrab goal rush", "Grab the goal rush mogo from where it was first dropped off"),
	jas::autobuildermodules("left mogo", "Grab the mogo on the left side of the field"),
	jas::autobuildermodules("right mogo", "Grab the mogo on the right side of the field"),
	jas::autobuildermodules("drop mogo", "Drop the currently held mogo, and stop running the intake"),
	jas::autobuildermodules("left corner", "Grab the two alliance rings in the left corner"),
	jas::autobuildermodules("right corner", "Grab the two alliance rings in the right corner"),
	jas::autobuildermodules("red mid two", "Assuming a red alliance, grab the two rings at the intersection between alliances"),
	jas::autobuildermodules("blue mid two", "Assuming a blue alliance, grab the two rings at the intersection between alliances"),
	jas::autobuildermodules("left bottom ring", "Grab the bottom ring of the left side ring stack"),
	jas::autobuildermodules("mid top ring", "Grab the top ring of the mid-field ring stack"),
	jas::autobuildermodules("right bottom ring", "Grab the bottom ring of the right side ring stack"),
	jas::autobuildermodules("ladder", "Touch the ladder")};
vector<jas::autobuildermodules> manmodules{};

lv_obj_t *mbuilderoverlay = lv_img_create(manbuilder);
lv_obj_t *pageswitchup = lv_img_create(manbuilder);
lv_obj_t *pageswitchdown = lv_img_create(autobuilder);
lv_obj_t *manbuilderlist = lv_table_create(manbuilder);
lv_obj_t *manlistlabel = lv_label_create(manbuilder);
lv_obj_t *manbuilderpath = lv_table_create(manbuilder);
lv_obj_t *manpathlabel = lv_label_create(manbuilder);
lv_obj_t *infobtn = lv_label_create(manbuilder);
lv_obj_t *addmodule = lv_label_create(manbuilder);
lv_obj_t *delmodule = lv_label_create(manbuilder);
lv_obj_t *allianceslider = lv_slider_create(manbuilder);
lv_obj_t *positionslider = lv_slider_create(manbuilder);
lv_obj_t *positionind = lv_label_create(manbuilder);
lv_obj_t *positiontoggle = lv_label_create(manbuilder);
lv_obj_t *pageupdwn;

static lv_style_t styletables;
static lv_style_t stylebtn;
static lv_style_t styleslider;

LV_IMG_DECLARE(manbuilderoverlay)

void tableupdate(lv_obj_t *targettable) {
	if(lv_obj_get_x(targettable) == 223) {
		l < getmodules.size() ? lv_table_set_cell_value(manbuilderlist, 0, 0, getmodules[l].Name.c_str()) : lv_table_set_cell_value(manbuilderlist, 0, 0, ""),
			noselection1 = true;
		l + 1 < getmodules.size() ? lv_table_set_cell_value(manbuilderlist, 1, 0, getmodules[l + 1].Name.c_str()) :
									lv_table_set_cell_value(manbuilderlist, 1, 0, "");
		l + 2 < getmodules.size() ? lv_table_set_cell_value(manbuilderlist, 2, 0, getmodules[l + 2].Name.c_str()) :
									lv_table_set_cell_value(manbuilderlist, 2, 0, "");
		l + 3 < getmodules.size() ? lv_table_set_cell_value(manbuilderlist, 3, 0, getmodules[l + 3].Name.c_str()) :
									lv_table_set_cell_value(manbuilderlist, 3, 0, "");
		l + 4 < getmodules.size() ? lv_table_set_cell_value(manbuilderlist, 4, 0, getmodules[l + 4].Name.c_str()) :
									lv_table_set_cell_value(manbuilderlist, 4, 0, "");
	} else {
		k < manmodules.size() ? lv_table_set_cell_value(manbuilderpath, 0, 0, manmodules[k].Name.c_str()) : lv_table_set_cell_value(manbuilderpath, 0, 0, ""),
			noselection2 = true;
		k + 1 < manmodules.size() ? lv_table_set_cell_value(manbuilderpath, 1, 0, manmodules[k + 1].Name.c_str()) :
									lv_table_set_cell_value(manbuilderpath, 1, 0, "");
		k + 2 < manmodules.size() ? lv_table_set_cell_value(manbuilderpath, 2, 0, manmodules[k + 2].Name.c_str()) :
									lv_table_set_cell_value(manbuilderpath, 2, 0, "");
		k + 3 < manmodules.size() ? lv_table_set_cell_value(manbuilderpath, 3, 0, manmodules[k + 3].Name.c_str()) :
									lv_table_set_cell_value(manbuilderpath, 3, 0, "");
		k + 4 < manmodules.size() ? lv_table_set_cell_value(manbuilderpath, 4, 0, manmodules[k + 4].Name.c_str()) :
									lv_table_set_cell_value(manbuilderpath, 4, 0, "");
	}
}

static void allianceselect(lv_event_t *e) {
	int target = lv_slider_get_value(allianceslider);
	const lv_color32_t colors[3] = {lv_color_hex(0xff2a00), lv_color_hex(0x5d5d5d), lv_color_hex(0x0066cc)};
	color = target;
	lv_obj_set_style_bg_color(allianceslider, colors[color], LV_PART_MAIN);
	lv_obj_set_style_bg_color(allianceslider, colors[color], LV_PART_INDICATOR);
}

static void selecttable(lv_event_t *e) {
	uint16_t row, col;
	lv_table_get_selected_cell(lv_event_get_target(e), &row, &col);
	if(lv_event_get_target(e) == manbuilderlist) {
		noselection1 = false;
		if(row == 0) selected1 = l;
		if(row == 1) selected1 = l + 1;
		if(row == 2) selected1 = l + 2;
		if(row == 3) selected1 = l + 3;
		if(row == 4) selected1 = l + 4;
	} else {
		noselection2 = false;
		if(row == 0) selected2 = k;
		if(row == 1) selected2 = k + 1;
		if(row == 2) selected2 = k + 2;
		if(row == 3) selected2 = k + 3;
		if(row == 4) selected2 = k + 4;
	}
	if(lv_event_get_target(e) == manbuilderlist && selected1 < getmodules.size())
		lv_label_set_text(manlistlabel, getmodules[selected1].Desc.c_str());
	else if(lv_event_get_target(e) == manbuilderlist)
		lv_label_set_text(manlistlabel, "No module selected");
	else if(selected2 < manmodules.size())
		lv_label_set_text(manpathlabel, manmodules[selected2].Desc.c_str());
	else
		lv_label_set_text(manpathlabel, "No module selected");
}

static void confirmchoice(lv_event_t *e) {
	lv_obj_t *target = lv_event_get_target(e);
	if(target == addmodule && selected1 < getmodules.size()) {
		if(noselection1 == true) return;
		manmodules.push_back(getmodules[selected1]);
	} else if(target == delmodule && selected2 < manmodules.size()) {
		if(noselection2 == true) return;
		manmodules.erase(manmodules.begin() + selected2);
		manmodules.shrink_to_fit();
		lv_label_set_text(manpathlabel, "No module selected");
	}
	tableupdate(target);

	if(strcmp(lv_table_get_cell_value(manbuilderpath, 0, 0), "") == 0) lv_event_send(pageupdwn, LV_EVENT_CLICKED, NULL);
}

static void pageupdate(lv_event_t *e) {
	lv_obj_t *target = lv_event_get_target(e);
	if(lv_obj_get_y(target) == 52) {
		if(lv_obj_get_x(target) == 223) {
			if(getmodules.size() % 5 == 0 && l < 5)
				l = (getmodules.size() - 5);
			else if(l < 5)
				l = (getmodules.size() - (getmodules.size() % 5));
			else
				l -= 5;
		} else {
			if(manmodules.size() % 5 == 0 && k < 5)
				k = (manmodules.size() - 5);
			else if(k < 5)
				k = (manmodules.size() - (manmodules.size() % 5));
			else
				k -= 5;
		}
	} else {
		if(lv_obj_get_x(target) == 223) {
			l += 5;
			if(l >= getmodules.size()) l = 0;
		} else {
			k += 5;
			if(k >= manmodules.size()) k = 0;
		}
	}
	tableupdate(target);
}

static void positionupdate(lv_event_t *e) {
	if(lv_event_get_target(e) == positionind) {
		if(lv_slider_get_value(positionslider) < 12)
			lv_slider_set_value(positionslider, 0, LV_ANIM_ON);
		else if(lv_slider_get_value(positionslider) < 36)
			lv_slider_set_value(positionslider, 24, LV_ANIM_ON);
		else if(lv_slider_get_value(positionslider) < 60)
			lv_slider_set_value(positionslider, 48, LV_ANIM_ON);
		else if(lv_slider_get_value(positionslider) < 84)
			lv_slider_set_value(positionslider, 72, LV_ANIM_ON);
		else if(lv_slider_get_value(positionslider) < 108)
			lv_slider_set_value(positionslider, 96, LV_ANIM_ON);
		else if(lv_slider_get_value(positionslider) < 132)
			lv_slider_set_value(positionslider, 120, LV_ANIM_ON);
		else
			lv_slider_set_value(positionslider, 144, LV_ANIM_ON);
	} else if(lv_event_get_target(e) == positiontoggle)
		positionorient = !positionorient;
	startposition = lv_slider_get_value(positionslider);
	lv_obj_set_pos(positionind, 303 + (startposition * 0.84722222222), 9);
	lv_label_set_text_fmt(positionind, "%d", lv_slider_get_value(positionslider));
	lv_label_set_text(positiontoggle, positionorient ? "f" : "r");
}

static void tableinfo(lv_event_t *e) {
	std::string manpath = "";
	for(int strings = 0; strings < manmodules.size(); strings++) {
		manpath = manpath.append("\n    " + manmodules[strings].Name);
	}
	lv_obj_t *buf = lv_label_create(manbuilder);
	lv_obj_add_flag(buf, LV_OBJ_FLAG_HIDDEN);
	lv_label_set_text_fmt(buf, "Team:\n    %s\nStart position:\n    %d inches, %s\nPath:%s",
						  (color == 0 ? "red" :
						   color == 1 ? "neutral" :
										"blue"),
						  startposition, positionorient ? "fwd" : "rev", manpath.c_str());

	lv_obj_t *manpathinfo = lv_msgbox_create(NULL, "path info", lv_label_get_text(buf), NULL, true);
	lv_obj_align(manpathinfo, LV_ALIGN_CENTER, 0, 0);
	lv_obj_add_style(manpathinfo, &styletables, LV_PART_MAIN);
	lv_obj_add_style(lv_msgbox_get_close_btn(manpathinfo), &styletables, LV_PART_MAIN);
	lv_obj_set_style_border_width(manpathinfo, 2, LV_PART_MAIN);
	lv_obj_set_style_border_color(manpathinfo, lv_color_hex(0xcfffe9), LV_PART_MAIN);
	lv_obj_set_style_text_align(manpathinfo, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
	lv_obj_set_style_text_font(manpathinfo, &lv_font_montserrat_12, LV_PART_MAIN);
	lv_obj_set_style_text_font(lv_msgbox_get_close_btn(manpathinfo), &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_set_style_text_font(lv_msgbox_get_title(manpathinfo), &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_del(buf);
	//autonomous(); //comment this out
}

static void screenchange(lv_event_t *e) {
	activescreen = !activescreen;
	lv_obj_set_tile(mainscreen, activescreen ? manbuilder : autobuilder, LV_ANIM_ON);
}

lv_event_cb_t pageUpdate = pageupdate;
lv_event_cb_t confirmChoice = confirmchoice;
lv_event_cb_t selectTable = selecttable;
lv_event_cb_t tableInfo = tableinfo;
lv_event_cb_t allianceSelect = allianceselect;
lv_event_cb_t positionUpdate = positionupdate;
lv_event_cb_t screenChange = screenchange;

void manualbuilderinit() {
	lv_style_init(&styletables);
	lv_style_set_border_width(&styletables, 0);
	lv_style_set_outline_width(&styletables, 1);
	lv_style_set_outline_color(&styletables, lv_color_hex(0x071808));
	lv_style_set_radius(&styletables, 0);
	lv_style_set_text_color(&styletables, lv_color_hex(0xcfffe9));
	lv_style_set_text_align(&styletables, LV_TEXT_ALIGN_CENTER);
	lv_style_set_bg_opa(&styletables, 255);
	lv_style_set_bg_color(&styletables, lv_color_hex(0x071808));

	lv_style_init(&stylebtn);
	lv_style_set_border_width(&stylebtn, 2);
	lv_style_set_radius(&stylebtn, 16);
	lv_style_set_border_color(&stylebtn, lv_color_hex(0x071808));
	lv_style_set_text_color(&stylebtn, lv_color_hex(0x071808));
	lv_style_set_bg_color(&stylebtn, lv_color_hex(0xcfffe9));
	lv_style_set_text_align(&stylebtn, LV_TEXT_ALIGN_CENTER);
	lv_style_set_bg_opa(&stylebtn, 255);
	lv_style_set_pad_top(&stylebtn, -1);
	lv_style_set_pad_left(&stylebtn, 1);
	lv_style_set_img_recolor_opa(&stylebtn, 255);

	lv_style_init(&styleslider);
	lv_style_set_border_width(&styleslider, 3);
	lv_style_set_outline_width(&styleslider, 2);
	lv_style_set_radius(&styleslider, 16);
	lv_style_set_border_color(&styleslider, lv_color_hex(0x071808));
	lv_style_set_outline_color(&styleslider, lv_color_hex(0xcfffe9));
	lv_style_set_bg_color(&styleslider, lv_color_hex(0xcfffe9));
	lv_style_set_text_align(&styleslider, LV_TEXT_ALIGN_CENTER);
	lv_style_set_bg_opa(&styleslider, 255);

	lv_img_set_src(mbuilderoverlay, &manbuilderoverlay);
	lv_img_set_src(pageswitchup, LV_SYMBOL_UP);
	lv_img_set_src(pageswitchdown, LV_SYMBOL_DOWN);
	lv_obj_align(pageswitchup, LV_ALIGN_TOP_MID, 0, 0);
	lv_obj_align(pageswitchdown, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_move_foreground(mbuilderoverlay);
	lv_obj_move_foreground(pageswitchup);
	lv_obj_move_foreground(pageswitchdown);

	lv_label_set_text(addmodule, LV_SYMBOL_OK);
	lv_label_set_text(delmodule, LV_SYMBOL_CLOSE);
	lv_label_set_text(infobtn, "?");
	lv_label_set_text(manlistlabel, "No module selected");
	lv_label_set_text(manpathlabel, "No module selected");

	lv_slider_set_range(allianceslider, 0, 2);
	lv_slider_set_range(positionslider, 0, 144);
	lv_slider_set_value(allianceslider, 1, LV_ANIM_OFF);
	lv_slider_set_value(positionslider, 72, LV_ANIM_OFF);

	lv_obj_set_size(addmodule, 47, 47);
	lv_obj_set_size(delmodule, 47, 47);
	lv_obj_set_size(manlistlabel, 158, 47);
	lv_obj_set_size(manpathlabel, 158, 47);
	lv_obj_set_size(infobtn, 17, 17);
	lv_obj_set_size(allianceslider, 119, 18);
	lv_obj_set_size(positionslider, 122, 18);
	lv_obj_set_size(positiontoggle, 17, 17);
	lv_obj_set_width(positionind, 22);

	lv_obj_set_pos(addmodule, 168, 188);
	lv_obj_set_pos(delmodule, 405, 188);
	lv_obj_set_pos(manlistlabel, 5, 188);
	lv_obj_set_pos(manpathlabel, 242, 188);
	lv_obj_set_pos(infobtn, 456, 31);
	lv_obj_set_pos(allianceslider, 48, 5);
	lv_obj_set_pos(positionslider, 313, 5);
	lv_obj_set_pos(positiontoggle, 220, 31);

	lv_obj_add_flag(pageswitchup, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(pageswitchdown, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(addmodule, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(delmodule, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(positiontoggle, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(positionind, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(infobtn, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_clear_flag(manbuilder, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_style(addmodule, &styletables, LV_PART_MAIN);
	lv_obj_add_style(delmodule, &styletables, LV_PART_MAIN);
	lv_obj_add_style(manlistlabel, &styletables, LV_PART_MAIN);
	lv_obj_add_style(manpathlabel, &styletables, LV_PART_MAIN);
	lv_obj_add_style(allianceslider, &styleslider, LV_PART_MAIN);
	lv_obj_add_style(allianceslider, &styleslider, LV_PART_INDICATOR);
	lv_obj_add_style(allianceslider, &styleslider, LV_PART_KNOB);
	lv_obj_add_style(positionslider, &styleslider, LV_PART_MAIN);
	lv_obj_add_style(positionslider, &styleslider, LV_PART_INDICATOR);
	lv_obj_add_style(positionslider, &styleslider, LV_PART_KNOB);
	lv_obj_add_style(positionind, &styleslider, LV_PART_MAIN);
	lv_obj_add_style(positiontoggle, &stylebtn, LV_PART_MAIN);
	lv_obj_add_style(infobtn, &stylebtn, LV_PART_MAIN);

	lv_obj_set_style_outline_width(addmodule, 0, LV_PART_MAIN);
	lv_obj_set_style_outline_width(delmodule, 0, LV_PART_MAIN);
	lv_obj_set_style_outline_width(positionind, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(positionind, 0, LV_PART_MAIN);
	lv_obj_set_style_radius(positiontoggle, 3, LV_PART_MAIN);
	lv_obj_set_style_radius(infobtn, 3, LV_PART_MAIN);
	lv_obj_set_style_transform_width(positionind, 2, LV_STATE_PRESSED);
	lv_obj_set_style_transform_height(positionind, 2, LV_STATE_PRESSED);
	lv_obj_set_style_transform_width(positiontoggle, 2, LV_STATE_PRESSED);
	lv_obj_set_style_transform_height(positiontoggle, 2, LV_STATE_PRESSED);
	lv_obj_set_style_transform_width(infobtn, 2, LV_STATE_PRESSED);
	lv_obj_set_style_transform_height(infobtn, 2, LV_STATE_PRESSED);
	lv_obj_set_style_pad_all(allianceslider, 0, LV_PART_KNOB);
	lv_obj_set_style_pad_all(positionslider, 0, LV_PART_KNOB);
	lv_obj_set_style_pad_all(manlistlabel, 3, LV_PART_MAIN);
	lv_obj_set_style_pad_all(manpathlabel, 3, LV_PART_MAIN);
	lv_obj_set_style_pad_hor(allianceslider, 2, LV_PART_KNOB);
	lv_obj_set_style_pad_hor(positionslider, 2, LV_PART_KNOB);

	lv_obj_set_style_text_align(manlistlabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
	lv_obj_set_style_text_align(manpathlabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
	lv_obj_set_style_text_font(positionind, &lv_font_montserrat_10, LV_PART_MAIN);
	lv_obj_set_style_text_font(addmodule, &lv_font_montserrat_48, LV_STATE_PRESSED);
	lv_obj_set_style_text_font(delmodule, &lv_font_montserrat_48, LV_STATE_PRESSED);
	lv_obj_set_style_text_color(positionind, lv_color_hex(0x071808), LV_PART_MAIN);
	lv_obj_set_style_text_opa(addmodule, 128, LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(delmodule, 128, LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(pageswitchup, 128, LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(pageswitchdown, 128, LV_STATE_PRESSED);
	lv_obj_set_style_bg_color(addmodule, lv_color_hex(0x10da1b), LV_PART_MAIN);
	lv_obj_set_style_bg_color(delmodule, lv_color_hex(0xe62f2f), LV_PART_MAIN);
	lv_obj_set_style_bg_color(positionslider, lv_color_hex(0x00b5bc), LV_PART_MAIN);
	lv_obj_set_style_bg_color(positionslider, lv_color_hex(0xdb8826), LV_PART_INDICATOR);
	lv_obj_set_style_bg_opa(positionind, 0, LV_PART_MAIN);

	lv_obj_add_event_cb(pageswitchup, screenChange, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(pageswitchdown, screenChange, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(addmodule, confirmChoice, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(delmodule, confirmChoice, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(allianceslider, allianceSelect, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_add_event_cb(positionslider, positionUpdate, LV_EVENT_VALUE_CHANGED, NULL);
	lv_obj_add_event_cb(positionind, positionUpdate, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(positiontoggle, positionUpdate, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(infobtn, tableInfo, LV_EVENT_CLICKED, NULL);

	lv_obj_move_foreground(allianceslider);
	lv_obj_move_foreground(positionslider);
	lv_obj_move_foreground(positionind);
	lv_obj_move_foreground(positiontoggle);
	lv_obj_move_foreground(infobtn);

	for(int i = 0; i < 2; i++) {
		lv_obj_add_style(i < 1 ? manbuilderlist : manbuilderpath, &styletables, LV_PART_MAIN);
		lv_obj_add_style(i < 1 ? manbuilderlist : manbuilderpath, &styletables, LV_PART_ITEMS);
		lv_obj_set_style_outline_color(i < 1 ? manbuilderlist : manbuilderpath, lv_color_hex(0x032415), LV_PART_ITEMS);
		lv_obj_set_style_pad_top(i < 1 ? manbuilderlist : manbuilderpath, 8, LV_PART_ITEMS);
		lv_obj_set_style_pad_bottom(i < 1 ? manbuilderlist : manbuilderpath, 6, LV_PART_ITEMS);
		lv_obj_set_style_pad_left(i < 1 ? manbuilderlist : manbuilderpath, 4, LV_PART_ITEMS);
		lv_obj_set_style_text_align(i < 1 ? manbuilderlist : manbuilderpath, LV_TEXT_ALIGN_LEFT, LV_PART_ITEMS);
		lv_obj_add_event_cb(i < 1 ? manbuilderlist : manbuilderpath, selectTable, LV_EVENT_VALUE_CHANGED, NULL);
		lv_table_set_col_width(i < 1 ? manbuilderlist : manbuilderpath, 0, 230);
		lv_obj_move_background(i < 1 ? manbuilderlist : manbuilderpath);
		lv_obj_set_size(i < 1 ? manbuilderlist : manbuilderpath, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
		lv_obj_set_pos(i < 1 ? manbuilderlist : manbuilderpath, i < 1 ? 5 : 242, 31);
	}

	for(int i = 0; i < 4; i++) {
		pageupdwn = lv_btn_create(manbuilder);
		lv_obj_add_style(pageupdwn, &stylebtn, LV_PART_MAIN);
		lv_obj_clear_flag(pageupdwn, LV_OBJ_FLAG_SCROLLABLE);
		lv_obj_set_size(pageupdwn, 11, 58);
		lv_obj_set_pos(pageupdwn, i % 2 == 0 ? 223 : 460, i < 2 ? 52 : 120);
		lv_obj_add_event_cb(pageupdwn, pageUpdate, LV_EVENT_CLICKED, NULL);
		lv_obj_update_layout(pageupdwn);
		tableupdate(pageupdwn);
	}
	lv_event_send(allianceslider, LV_EVENT_VALUE_CHANGED, NULL);
	lv_event_send(positionslider, LV_EVENT_VALUE_CHANGED, NULL);
}

void mancallback() {
	chassis.odom_pose_set({startposition * okapi::inch, 21_in, positionorient ? 0_deg : 180_deg});
	cout << util::to_string_with_precision(chassis.odom_x_get()) << endl;
	if(color != 1)
		ringsorting =
			pros::c::task_create(ringsensTask, color == 0 ? (void *)"0" : (void *)"1", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "ring sorting");
	for(int module_it = 0; module_it < manmodules.size(); module_it++) {
		if(manmodules[module_it].Name == "goal rush")
			// cout << "goal_rush" << endl;
			goal_rush();
		else if(manmodules[module_it].Name == "alliance stake")
			// cout << "alliancestake" << endl;
			alliancestake();
		else if(manmodules[module_it].Name == "blue regrab goal rush")
			// cout << "blue_gr_mogo" << endl;
			blue_gr_mogo();
		else if(manmodules[module_it].Name == "red regrab goal rush")
			// cout << "red_gr_mogo" << endl;
			red_gr_mogo();
		else if(manmodules[module_it].Name == "left mogo")
			// cout << "left_mogo" << endl;
			left_mogo();
		else if(manmodules[module_it].Name == "right mogo")
			// cout << "right_mogo" << endl;
			right_mogo();
		else if(manmodules[module_it].Name == "left corner")
			// cout << "left_corner" << endl;
			left_corner();
		else if(manmodules[module_it].Name == "right corner")
			// cout << "right_corner" << endl;
			right_corner();
		else if(manmodules[module_it].Name == "red mid two")
			// cout << "red_ring_rush" << endl;
			red_ring_rush();
		else if(manmodules[module_it].Name == "blue mid two")
			// cout << "blue_ring_rush" << endl;
			blue_ring_rush();
		else if(manmodules[module_it].Name == "left bottom ring")
			// cout << "left_btm_ring" << endl;
			left_btm_ring();
		else if(manmodules[module_it].Name == "mid top ring")
			// cout << "mid_top_ring" << endl;
			mid_top_ring();
		else if(manmodules[module_it].Name == "right bottom ring")
			// cout << "right_btm_ring" << endl;
			right_btm_ring();
		else if(manmodules[module_it].Name == "ladder")
			// cout << "ladder" << endl;
			ladder();
		else if(manmodules[module_it].Name == "drop mogo") {
			// cout << "drop mogo" << endl;
			intake.move(0);
			mogomech.set(false);
		}
	}
	if(color != 1) pros::c::task_delete(ringsorting);
}
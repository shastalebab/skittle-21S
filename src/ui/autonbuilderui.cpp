#include "main.h"  // IWYU pragma: keep

int mogo1;
int mogo2;
int redblubuilder(1);
int posnegbuilder(3);
int goalrushbuilder(5);
bool allianceringbuilder(true);
bool useredblu = true;
bool useposneg = true;
bool usegoalrush = false;
int *rings;

vector<std::string> modules;

lv_obj_t *builderoverlay;
lv_obj_t *Mogo1;
lv_obj_t *Mogo1back;
lv_obj_t *Mogo2;
lv_obj_t *Mogo2back;
lv_obj_t *Pos;
lv_obj_t *Neg;
lv_obj_t *RedBlu;
lv_obj_t *AllianceRing;
lv_obj_t *GoalRush;
lv_obj_t *builderringind;
lv_obj_t *AutonBaseInfo;
lv_obj_t *AutonPathInfo;
lv_obj_t *AutonBaseInfoTxt;
lv_obj_t *AutonPathInfoTxt;

static lv_style_t styleindbuilder;
static lv_style_t styletextbuilder;

LV_IMG_DECLARE(autonbuilderoverlay);

void updatepath() {
	// Reset/create and assign all variables used to build the path
	int finmogo1 = mogo1 > 0 ? mogo1 : mogo2;
	int finmogo2 = mogo1 > 0 ? mogo2 : 0;
	if((allianceringbuilder == true ? 1 : 0) + finmogo1 + finmogo2 > 10) finmogo2 -= ((allianceringbuilder == true ? 1 : 0) + finmogo1 + finmogo2 - 10);
	int singlesiter = 0;
	int doublesiter = 0;
	int cornersiter = 0;
	int usefinmogo1 = finmogo1;
	int usefinmogo2 = finmogo2;
	useredblu = redblubuilder == 0 ? true : false;
	useposneg = posnegbuilder == 3 ? true : false;
	usegoalrush = goalrushbuilder == 6 ? true : false;
	bool usemidtwo = useposneg;
	bool setside = true;
	std::string path;

	// Empty the modules vector and create temporary singles/doubles vector
	modules.clear();
	vector<std::string> singles{"left bottom ring", "mid top ring", "right bottom ring"};
	vector<std::string> doubles{"left corner", "right corner"};

	// Set the selection info box
	lv_label_set_text_fmt(AutonBaseInfoTxt, "Team: %s\nCorner: %s\nMogo 1: %d\nMogo 2: %d\nG.Rush: %s\nA.Stke: %s\nStart pos: %d inches",
						  (useredblu == true ? "Red" : "Blue"), (useposneg == true ? "Neg" : "Pos"), finmogo1, finmogo2, (usegoalrush == true ? "On" : "Off"),
						  (allianceringbuilder == true ? "On" : "Off"),
						  usegoalrush == true ? (useredblu == useposneg ? 107 : 37) : (useredblu == useposneg ? 96 : 48));
	// Start by adding the goal rush and then the alliance stake, if either are available
	if(usegoalrush == true) modules.push_back(useredblu == useposneg ? "blue goal rush" : "red goal rush");
	if(allianceringbuilder == true) modules.push_back("alliance stake");

	// First mogo logic
	if(usefinmogo1 > 0) {
		// Grab a mobile goal
		modules.push_back(usegoalrush == true ? (useredblu == useposneg ? "blue regrab goal rush" : "red regrab goal rush") :
												(useredblu == useposneg ? "left mogo" : "right mogo"));
		// Set the side that the auton is on, and toggle whether the mid two rings should be used
		setside = useredblu == useposneg;
		usemidtwo = useredblu == useposneg ? setside : !setside;
		// Fill mogo
		for(int ringscore = 0; ringscore < usefinmogo1 - (allianceringbuilder == true ? 0 : 1); ringscore++) {
			// Check if doubles should be used
			if((finmogo1 - (allianceringbuilder == true ? 0 : 1)) % 2 == 0 && finmogo1 - (allianceringbuilder == true ? 0 : 1) != 0 && doublesiter < 3) {
				// Check whether the corner or the mid two should be attempted first, and in which order
				if((useposneg == false || usemidtwo == false) && cornersiter < 2) {
					modules.push_back(setside == true ? doubles.front() : doubles.back());
					doubles.erase(setside == true ? doubles.begin() : doubles.end());
					doubles.shrink_to_fit();
					cornersiter++;
					if((finmogo1 - (allianceringbuilder == true ? 2 : 3)) % 2 == 0 && finmogo1 - (allianceringbuilder == true ? 2 : 3) != 0 &&
					   doublesiter + 1 < 3)
						setside = !setside;
				} else if(usemidtwo == true || cornersiter >= 2) {
					modules.push_back(useredblu == true ? "red mid two" : "blue mid two");
					usemidtwo = false;
				}
				// Decrease the variable counting how many rings are scored
				finmogo1 -= 2;
				doublesiter++;
				// Check if singles should be used
			} else if((finmogo1 - (allianceringbuilder == true ? 0 : 1) != 0) && singlesiter < 3) {
				// Add singles in order depending on side
				modules.push_back(setside == true ? singles.front() : singles.back());
				singles.erase(setside == true ? singles.begin() : singles.end());
				singles.shrink_to_fit();
				finmogo1--;
				singlesiter++;
			}
		}
	}

	// Second mogo logic
	if(usefinmogo2 > 0) {
		// Drop previously held mogo
		modules.push_back("drop mogo");
		// Grab a mobile goal
		modules.push_back((useredblu == useposneg) == usegoalrush ? "left mogo" : "right mogo");
		// Set the side that the auton is on, and toggle whether the mid two rings should be used
		setside = (useredblu == useposneg) == usegoalrush;
		usemidtwo = useredblu == useposneg ? setside : !setside;
		// Fill mogo
		for(int ringscore = 0; ringscore < usefinmogo2; ringscore++) {
			// Check if doubles should be used
			if((finmogo2) % 2 == 0 && finmogo2 != 0 && doublesiter < 3) {
				// Check whether the corner or the mid two should be attempted first, and in which order
				if((useposneg == false || usemidtwo == false) && cornersiter < 2) {
					modules.push_back(setside == true ? doubles.front() : doubles.back());
					doubles.erase(setside == true ? doubles.begin() : doubles.end());
					doubles.shrink_to_fit();
					cornersiter++;
					if((finmogo2 - 2) % 2 == 0 && finmogo2 - 2 != 0 && doublesiter + 1 < 3) setside = !setside;
				} else if(usemidtwo == true || cornersiter >= 2) {
					modules.push_back(useredblu == true ? "red mid two" : "blue mid two");
					usemidtwo = false;
				}
				// Decrease the variable counting how many rings are scored
				finmogo2 -= 2;
				doublesiter++;
				// Check if singles should be used
			} else if((finmogo2 != 0) && singlesiter < 3) {
				// Add singles in order depending on side
				modules.push_back(setside == true ? singles.front() : singles.back());
				singles.erase(setside == true ? singles.begin() : singles.end());
				singles.shrink_to_fit();
				finmogo2--;
				singlesiter++;
			}
		}
	}

	// End path with ladder
	modules.push_back("ladder");

	// In cases where the single selected was followed up immediately by the mid two rings, switch them around in the path
	for(int i = 0; i < modules.size() - 1; i++) {
		if((modules[i] == "left bottom ring" || modules[i] == "right bottom ring") && (modules[i + 1] == "red mid two" || modules[i + 1] == "blue mid two")) {
			auto two = modules[i];
			auto one = modules[i + 1];
			modules[i] = one;
			modules[i + 1] = two;
		}
	}

	// Update the path info box with all of the modules
	for(int strings = 0; strings < modules.size(); strings++) {
		path = path.append(strings == modules.size() - 1 ? modules[strings] : modules[strings] + "\n");
	}

	lv_label_set_text_fmt(AutonPathInfoTxt, "%s", path.c_str());
}

static void mogopress(lv_event_t *e) {
	// Increase visually (and in the program) how many rings are on each mogo
	lv_event_get_target(e) == Mogo1 || lv_event_get_target(e) == Mogo1back ? rings = &mogo1 : rings = &mogo2;
	*rings = (*rings + 1) % 7;
	lv_obj_set_size(Mogo1, 67, 108 - (18 * mogo1));
	lv_obj_set_size(Mogo2, 67, 108 - (18 * mogo2));

	updatepath();
}

static void specify(lv_event_t *e) {
	// Alter the screen elements to match all of the specified conditions, and update all variables to match.
	lv_obj_t *target = lv_event_get_target(e);

	if(target == RedBlu) redblubuilder == 0 ? redblubuilder = 1 : redblubuilder = 0;
	if(target == AllianceRing) allianceringbuilder == false ? allianceringbuilder = true : allianceringbuilder = false;
	if(target == GoalRush && posnegbuilder == 4) {
		goalrushbuilder == 5 ? goalrushbuilder = 6 : goalrushbuilder = 5;
	}

	if(target == Pos) {
		posnegbuilder = 4;
		lv_obj_set_style_bg_color(Neg, colortable[5], LV_PART_MAIN);
		lv_obj_set_style_bg_color(Pos, colortable[posnegbuilder], LV_PART_MAIN);
		lv_obj_set_style_bg_opa(GoalRush, 255, LV_PART_MAIN);
	}
	if(target == Neg) {
		posnegbuilder = 3;
		lv_obj_set_style_bg_color(Pos, colortable[5], LV_PART_MAIN);
		lv_obj_set_style_bg_color(Neg, colortable[posnegbuilder], LV_PART_MAIN);
		lv_obj_set_style_bg_opa(GoalRush, 0, LV_PART_MAIN);
		goalrushbuilder = 5;
	}

	lv_obj_set_style_bg_color(RedBlu, colortable[redblubuilder], LV_PART_MAIN);
	lv_obj_set_style_bg_color(Mogo1back, colortable[redblubuilder], LV_PART_MAIN);
	lv_obj_set_style_bg_color(Mogo2back, colortable[redblubuilder], LV_PART_MAIN);
	lv_obj_set_style_bg_color(GoalRush, colortable[goalrushbuilder], LV_PART_MAIN);
	allianceringbuilder == true ? lv_obj_set_style_bg_opa(AllianceRing, 0, LV_PART_MAIN) : lv_obj_set_style_bg_opa(AllianceRing, 255, LV_PART_MAIN);

	updatepath();
}

static void getinfo(lv_event_t *e) {
	// Create a message box that displays the text from the text box which was clicked, but in higher resolution
	lv_obj_t *target = lv_event_get_current_target(e);
	lv_obj_t *autonbuildinfo = lv_msgbox_create(NULL, (target == AutonBaseInfo ? "selection info" : "path info"),
												lv_label_get_text(target == AutonBaseInfo ? AutonBaseInfoTxt : AutonPathInfoTxt), NULL, true);
	lv_obj_add_style(autonbuildinfo, &styletextbuilder, LV_PART_MAIN);
	lv_obj_add_style(lv_msgbox_get_close_btn(autonbuildinfo), &styletextbuilder, LV_PART_MAIN);

	lv_obj_set_style_border_opa(lv_msgbox_get_close_btn(autonbuildinfo), 0, LV_PART_MAIN);

	lv_obj_set_style_text_font(autonbuildinfo, target == AutonBaseInfo ? &lv_font_montserrat_20 : &lv_font_montserrat_12, LV_PART_MAIN);
	lv_obj_set_style_text_font(lv_msgbox_get_close_btn(autonbuildinfo), &lv_font_montserrat_20, LV_PART_MAIN);
	lv_obj_set_style_text_font(lv_msgbox_get_title(autonbuildinfo), &lv_font_montserrat_20, LV_PART_MAIN);

	lv_obj_align(autonbuildinfo, LV_ALIGN_CENTER, 0, 0);
}

// Create events
lv_event_cb_t mogoPress = mogopress;
lv_event_cb_t Specify = specify;
lv_event_cb_t getInfo = getinfo;

void autonbuilderinit() {
	// Initialize automatic auton builder screen
	activescreen = false;

	// Create screen elements
	builderoverlay = lv_img_create(autobuilder);
	Mogo1 = lv_btn_create(autobuilder);
	Mogo1back = lv_btn_create(autobuilder);
	Mogo2 = lv_btn_create(autobuilder);
	Mogo2back = lv_btn_create(autobuilder);
	Pos = lv_btn_create(autobuilder);
	Neg = lv_btn_create(autobuilder);
	RedBlu = lv_btn_create(autobuilder);
	AllianceRing = lv_btn_create(autobuilder);
	GoalRush = lv_btn_create(autobuilder);
	builderringind = lv_obj_create(autobuilder);
	AutonBaseInfo = lv_obj_create(autobuilder);
	AutonPathInfo = lv_obj_create(autobuilder);
	AutonBaseInfoTxt = lv_label_create(AutonBaseInfo);
	AutonPathInfoTxt = lv_label_create(AutonPathInfo);

	// Initialize indicator style
	lv_style_init(&styleindbuilder);
	lv_style_set_border_width(&styleindbuilder, 0);
	lv_style_set_radius(&styleindbuilder, 0);
	lv_style_set_transform_width(&styleindbuilder, 0);
	lv_style_set_transform_height(&styleindbuilder, 0);

	// Initialize text style
	lv_style_init(&styletextbuilder);
	lv_style_set_border_color(&styletextbuilder, lv_color_hex(0xcfffe9));
	lv_style_set_border_width(&styletextbuilder, 2);
	lv_style_set_radius(&styletextbuilder, 0);
	lv_style_set_text_color(&styletextbuilder, lv_color_hex(0xcfffe9));
	lv_style_set_bg_color(&styletextbuilder, lv_color_hex(0x071808));
	lv_style_set_bg_opa(&styletextbuilder, 255);
	lv_style_set_text_font(&styletextbuilder, &lv_font_montserrat_10);
	lv_style_set_pad_all(&styletextbuilder, 2);

	// Set overlay image
	lv_img_set_src(builderoverlay, &autonbuilderoverlay);  //"S:autonbuilderoverlay.bin"

	// Set all object positions
	lv_obj_set_pos(Mogo1, 49, 90);
	lv_obj_set_pos(Mogo1back, 49, 90);
	lv_obj_set_pos(Mogo2, 364, 90);
	lv_obj_set_pos(Mogo2back, 364, 90);
	lv_obj_set_pos(Pos, 142, 69);
	lv_obj_set_pos(Neg, 290, 88);
	lv_obj_set_pos(RedBlu, 204, 13);
	lv_obj_set_pos(AllianceRing, 204, 59);
	lv_obj_set_pos(GoalRush, 216, 153);
	lv_obj_set_pos(builderringind, 220, 100);
	lv_obj_set_pos(AutonBaseInfo, 129, 128);
	lv_obj_set_pos(AutonPathInfo, 264, 128);

	// Set all object sizes
	lv_obj_set_size(Mogo1, 67, 105);
	lv_obj_set_size(Mogo1back, 67, 105);
	lv_obj_set_size(Mogo2, 67, 105);
	lv_obj_set_size(Mogo2back, 67, 105);
	lv_obj_set_size(Pos, 48, 48);
	lv_obj_set_size(Neg, 48, 11);
	lv_obj_set_size(RedBlu, 72, 74);
	lv_obj_set_size(AllianceRing, 72, 17);
	lv_obj_set_size(GoalRush, 48, 73);
	lv_obj_set_size(builderringind, 40, 40);
	lv_obj_set_size(AutonBaseInfo, 87, 65);
	lv_obj_set_size(AutonPathInfo, 87, 65);

	// Add styles to all objects
	lv_obj_add_style(Mogo1, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(Mogo1back, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(Mogo2, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(Mogo2back, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(Mogo1, &styleindbuilder, LV_STATE_PRESSED);
	lv_obj_add_style(Mogo1back, &styleindbuilder, LV_STATE_PRESSED);
	lv_obj_add_style(Mogo2, &styleindbuilder, LV_STATE_PRESSED);
	lv_obj_add_style(Mogo2back, &styleindbuilder, LV_STATE_PRESSED);
	lv_obj_add_style(Pos, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(Neg, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(RedBlu, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(AllianceRing, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(GoalRush, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(builderringind, &styleindbuilder, LV_PART_MAIN);
	lv_obj_add_style(AutonBaseInfo, &styletextbuilder, LV_PART_MAIN);
	lv_obj_add_style(AutonPathInfo, &styletextbuilder, LV_PART_MAIN);

	// Modify object flags
	lv_obj_clear_flag(AutonBaseInfo, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_clear_flag(AutonPathInfo, LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_add_flag(AutonBaseInfo, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(AutonPathInfo, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(AutonBaseInfoTxt, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(AutonPathInfoTxt, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(AutonBaseInfoTxt, LV_OBJ_FLAG_EVENT_BUBBLE);
	lv_obj_add_flag(AutonPathInfoTxt, LV_OBJ_FLAG_EVENT_BUBBLE);

	// Change object background colors
	lv_obj_set_style_bg_color(Mogo1, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
	lv_obj_set_style_bg_color(Mogo2, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
	lv_obj_set_style_bg_color(AllianceRing, lv_color_hex(0x5d5d5d), LV_PART_MAIN);

	// Add clicking animation to the text boxes
	lv_obj_set_style_transform_width(AutonBaseInfo, 2, LV_STATE_PRESSED);
	lv_obj_set_style_transform_height(AutonBaseInfo, 2, LV_STATE_PRESSED);
	lv_obj_set_style_transform_width(AutonPathInfo, 2, LV_STATE_PRESSED);
	lv_obj_set_style_transform_height(AutonPathInfo, 2, LV_STATE_PRESSED);

	// Rearrange mogo layers
	lv_obj_move_background(Mogo1back);
	lv_obj_move_background(Mogo2back);

	// Add events
	lv_obj_add_event_cb(Mogo1, mogoPress, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Mogo1back, mogoPress, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Mogo2, mogoPress, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Mogo2back, mogoPress, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Pos, Specify, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(Neg, Specify, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(RedBlu, Specify, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(AllianceRing, Specify, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(GoalRush, Specify, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(AutonBaseInfo, getInfo, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(AutonPathInfo, getInfo, LV_EVENT_CLICKED, NULL);

	// Rearrange all objects on screen
	lv_obj_move_foreground(Mogo1);
	lv_obj_move_foreground(Mogo2);
	lv_obj_move_foreground(AllianceRing);
	lv_obj_move_foreground(builderoverlay);
	lv_obj_move_foreground(AutonBaseInfo);
	lv_obj_move_foreground(AutonPathInfo);

	// Send events for a default state
	lv_event_send(RedBlu, LV_EVENT_CLICKED, NULL);
	lv_event_send(Neg, LV_EVENT_CLICKED, NULL);
	lv_event_send(AllianceRing, LV_EVENT_CLICKED, NULL);
}

void autocallback() {
	// Set the position and alliance of the auton based on the selection
	chassis.odom_pose_set({usegoalrush == true ? (useredblu == useposneg ? 37_in : 107_in) : (useredblu == useposneg ? 48_in : 96_in), 20_in,
						   usegoalrush == true ? 0_deg : 180_deg});
	allianceColor = useredblu ? Colors::RED : Colors::BLUE;
	for(int module_it = 0; module_it < modules.size(); module_it++) {
		if(modules[module_it] == "blue goal rush")
			blue_goal_rush();
		else if(modules[module_it] == "red goal rush")
			red_goal_rush();
		else if(modules[module_it] == "alliance stake")
			alliancestake();
		else if(modules[module_it] == "blue regrab goal rush")
			blue_gr_mogo();
		else if(modules[module_it] == "red regrab goal rush")
			red_gr_mogo();
		else if(modules[module_it] == "left mogo")
			left_mogo();
		else if(modules[module_it] == "right mogo")
			right_mogo();
		else if(modules[module_it] == "left corner")
			left_corner();
		else if(modules[module_it] == "right corner")
			right_corner();
		else if(modules[module_it] == "red mid two")
			red_ring_rush();
		else if(modules[module_it] == "blue mid two")
			blue_ring_rush();
		else if(modules[module_it] == "left bottom ring")
			left_btm_ring();
		else if(modules[module_it] == "mid top ring")
			mid_top_ring();
		else if(modules[module_it] == "right bottom ring")
			right_btm_ring();
		else if(modules[module_it] == "ladder")
			ladder();
		else if(modules[module_it] == "drop mogo")
			drop_mogo();
	}
}
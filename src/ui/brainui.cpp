#include "main.h"  // IWYU pragma: keep

using namespace jas;

int j = 0;
int selected = 0;
int redblustore = 0;
int posnegstore = 0;
int scrpage = 0;
vector<jasauton> jautoncurated = {};
bool noselection;
bool pageside[2]{true, false};
jas::jautontable j_auton_selector;

jasauton::jasauton() {
	AutonCall = nullptr;
	RedBluFilt = 2;
	PosNegFilt = 2;
	Name = "";
	Desc = "";
	Mogo1 = 0;
	Mogo2 = 0;
	AllyRing = false;
}

jasauton::jasauton(std::function<void()> autoncall, int redblufilt, int posnegfilt, std::string name, std::string desc, int mogo1, int mogo2, bool allyring) {
	AutonCall = autoncall;
	RedBluFilt = redblufilt;
	PosNegFilt = posnegfilt;
	Name = name;
	Desc = desc;
	Mogo1 = mogo1;
	Mogo2 = mogo2;
	AllyRing = allyring;
}

void jautontable::jautonpopulate(std::vector<jasauton> JasAutonTable) {
	jasautontable = {};
	jasautontable.assign(JasAutonTable.begin(), JasAutonTable.end());
}

static lv_style_t style;
static lv_style_t stylebtn;
static lv_style_t styleind;
static lv_style_t styleswitch;

lv_obj_t *mainscreen = lv_tileview_create(NULL);
lv_obj_t *autoselector = lv_tileview_add_tile(mainscreen, 0, 0, LV_DIR_NONE);
lv_obj_t *motortemps = lv_tileview_add_tile(mainscreen, 1, 0, LV_DIR_NONE);
lv_obj_t *autobuilder = lv_tileview_add_tile(mainscreen, 2, 0, LV_DIR_NONE);
lv_obj_t *manbuilder = lv_tileview_add_tile(mainscreen, 2, 1, LV_DIR_NONE);
lv_obj_t *overlay = lv_img_create(autoselector);
lv_obj_t *autonselectup = lv_btn_create(autoselector);
lv_obj_t *autonselectdown = lv_btn_create(autoselector);
lv_obj_t *redblu = lv_switch_create(autoselector);
lv_obj_t *posneg = lv_switch_create(autoselector);
lv_obj_t *negind = lv_obj_create(autoselector);
lv_obj_t *posind = lv_obj_create(autoselector);
lv_obj_t *redbluind = lv_obj_create(autoselector);
lv_obj_t *jauton = lv_table_create(autoselector);
lv_obj_t *selectedAuton = lv_label_create(autoselector);
lv_obj_t *selectedAutonbox = lv_obj_create(autoselector);
lv_obj_t *autondesc = lv_label_create(autoselector);
lv_obj_t *mogoring1 = lv_obj_create(autoselector);
lv_obj_t *mogoring2 = lv_obj_create(autoselector);
lv_obj_t *mogoringback1 = lv_obj_create(autoselector);
lv_obj_t *mogoringback2 = lv_obj_create(autoselector);
lv_obj_t *alliancering = lv_obj_create(autoselector);
lv_obj_t *ringind = lv_obj_create(autoselector);
lv_obj_t *pageswitch;

LV_IMG_DECLARE(brainuioverlay);

vector<lv_color32_t> colortable = {lv_color_hex(0xff2a00), lv_color_hex(0x0066cc), lv_color_hex(0x5d5d5d), lv_color_hex(0x00b5bc),
								   lv_color_hex(0xdb8826), lv_color_hex(0x5d5d5d), lv_color_hex(0x84f03b)};

void jautonrun() { jautoncurated[selected].AutonCall(); }

static void clear() {
	noselection = true;
	lv_obj_set_style_bg_color(redbluind, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
	lv_obj_set_style_bg_color(posind, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
	lv_obj_set_style_bg_color(negind, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
	lv_obj_set_style_bg_color(mogoring1, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
	lv_obj_set_style_bg_color(mogoring2, lv_color_hex(0x5d5d5d), LV_PART_MAIN);
	lv_label_set_text(selectedAuton, "No auton selected");
	lv_obj_clear_flag(alliancering, LV_OBJ_FLAG_HIDDEN);
}

static void listupdate() {
	j < jautoncurated.size() ? lv_table_set_cell_value(jauton, 0, 0, (jautoncurated[j].Name).c_str()) : lv_table_set_cell_value(jauton, 0, 0, "");
	j + 1 < jautoncurated.size() ? lv_table_set_cell_value(jauton, 1, 0, (jautoncurated[j + 1].Name).c_str()) : lv_table_set_cell_value(jauton, 1, 0, "");
	j + 2 < jautoncurated.size() ? lv_table_set_cell_value(jauton, 2, 0, (jautoncurated[j + 2].Name).c_str()) : lv_table_set_cell_value(jauton, 2, 0, "");
	j + 3 < jautoncurated.size() ? lv_table_set_cell_value(jauton, 3, 0, (jautoncurated[j + 3].Name).c_str()) : lv_table_set_cell_value(jauton, 3, 0, "");
}

static void jautoncurate(lv_event_t *e) {
	jautoncurated.clear();
	j = 0;
	lv_obj_has_state(redblu, LV_STATE_CHECKED) == true ? redblustore = 1 : redblustore = 0;
	lv_obj_has_state(posneg, LV_STATE_CHECKED) == true ? posnegstore = 1 : posnegstore = 0;
	for(int i = 0; i < j_auton_selector.jasautontable.size(); i++) {
		if((j_auton_selector.jasautontable[i].RedBluFilt == redblustore || j_auton_selector.jasautontable[i].RedBluFilt == 2) &&
		   (j_auton_selector.jasautontable[i].PosNegFilt == posnegstore || j_auton_selector.jasautontable[i].PosNegFilt == 2)) {
			jautoncurated.push_back(j_auton_selector.jasautontable[i]);
		}
	}
	listupdate();
}

static void selectauton(lv_event_t *e) {
	uint16_t row, col;
	lv_table_get_selected_cell(jauton, &row, &col);
	if(row == 0) selected = j;
	if(row == 1) selected = j + 1;
	if(row == 2) selected = j + 2;
	if(row == 3) selected = j + 3;
	clear();
	if(strcmp(lv_table_get_cell_value(jauton, row, 0), "") != 0) {
		noselection = false;
		lv_obj_has_state(redblu, LV_STATE_CHECKED) == true ? lv_obj_set_style_bg_color(redbluind, lv_color_hex(0x0066cc), LV_PART_MAIN) :
															 lv_obj_set_style_bg_color(redbluind, lv_color_hex(0xff2a00), LV_PART_MAIN);
		lv_obj_has_state(posneg, LV_STATE_CHECKED) == true ? lv_obj_set_style_bg_color(posind, lv_color_hex(0xdb8826), LV_PART_MAIN) :
															 lv_obj_set_style_bg_color(negind, lv_color_hex(0x00b5bc), LV_PART_MAIN);
		colortable[2] = lv_obj_get_style_bg_color(redbluind, LV_PART_MAIN);
		lv_label_set_text(selectedAuton, ((jautoncurated[selected].Desc).c_str()));
		lv_obj_set_style_bg_color(mogoring1, colortable[jautoncurated[selected].RedBluFilt], LV_PART_MAIN);
		lv_obj_set_size(mogoringback1, 46, (78 - (13 * jautoncurated[selected].Mogo1)));
		lv_obj_set_style_bg_color(mogoring2, colortable[jautoncurated[selected].RedBluFilt], LV_PART_MAIN);
		lv_obj_set_size(mogoringback2, 46, (78 - (13 * jautoncurated[selected].Mogo2)));
		jautoncurated[selected].AllyRing == true ? lv_obj_add_flag(alliancering, LV_OBJ_FLAG_HIDDEN) : lv_obj_clear_flag(alliancering, LV_OBJ_FLAG_HIDDEN);
		//jautonrun();  // comment this out
	}
}

static void updownbtn(lv_event_t *e) {
	if(lv_obj_has_state(autonselectup, LV_EVENT_CLICKED)) {
		if(jautoncurated.size() % 4 == 0 && j < 4)
			j = (jautoncurated.size() - 4);
		else if(j < 4)
			j = (jautoncurated.size() - (jautoncurated.size() % 4));
		else
			j -= 4;
	} else if(lv_obj_has_state(autonselectdown, LV_EVENT_CLICKED)) {
		j += 4;
		if(j >= jautoncurated.size()) j = 0;
	}
	listupdate();
}

lv_obj_t *screens[3]{autoselector, motortemps, autobuilder};

static void pageswitchbtn(lv_event_t *e) {
	const bool *getside = (bool *)lv_event_get_user_data(e);
	scrpage = *getside ? (scrpage + 1) % 3 : scrpage == 0 ? 2 : (scrpage - 1) % 3;
	lv_obj_set_tile(mainscreen, screens[scrpage], LV_ANIM_ON);
	lv_obj_set_parent(pageswitch, screens[scrpage]);
}

lv_event_cb_t jautonCurate = jautoncurate;
lv_event_cb_t selectAuton = selectauton;
lv_event_cb_t upDownBtn = updownbtn;
lv_event_cb_t pageSwitchBtn = pageswitchbtn;

void screeninit() {
	lv_style_init(&style);
	lv_style_set_border_width(&style, 0);
	lv_style_set_radius(&style, 0);
	lv_style_set_text_color(&style, lv_color_hex(0xcfffe9));
	lv_style_set_bg_color(&style, lv_color_hex(0x071808));

	lv_style_init(&stylebtn);
	lv_style_set_border_width(&stylebtn, 2);
	lv_style_set_radius(&stylebtn, 6);
	lv_style_set_border_color(&stylebtn, lv_color_hex(0x071808));
	lv_style_set_text_color(&stylebtn, lv_color_hex(0x071808));
	lv_style_set_bg_color(&stylebtn, lv_color_hex(0xcfffe9));
	lv_style_set_text_align(&stylebtn, LV_TEXT_ALIGN_CENTER);
	lv_style_set_bg_opa(&stylebtn, 255);

	lv_style_init(&styleind);
	lv_style_set_border_width(&styleind, 0);
	lv_style_set_radius(&styleind, 0);
	lv_style_set_bg_color(&styleind, lv_color_hex(0x5d5d5d));

	lv_style_init(&styleswitch);
	lv_style_set_border_width(&styleswitch, 3);
	lv_style_set_outline_width(&styleswitch, 2);
	lv_style_set_radius(&styleswitch, 16);
	lv_style_set_border_color(&styleswitch, lv_color_hex(0x071808));
	lv_style_set_outline_color(&styleswitch, lv_color_hex(0xcfffe9));

	lv_img_set_src(overlay, &brainuioverlay);  //"S:brainuioverlay.bin"
	lv_obj_move_foreground(overlay);
	lv_scr_load(mainscreen);
	lv_obj_add_style(mainscreen, &style, LV_PART_MAIN);
	lv_obj_set_scrollbar_mode(mainscreen, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_size(autonselectup, 11, 58);
	lv_obj_set_pos(autonselectup, 242, 50);
	lv_obj_set_size(autonselectdown, 11, 58);
	lv_obj_set_pos(autonselectdown, 242, 118);
	lv_obj_add_style(autonselectup, &stylebtn, LV_PART_MAIN);
	lv_obj_add_style(autonselectdown, &stylebtn, LV_PART_MAIN);
	lv_obj_set_size(ringind, 117, 117);
	lv_obj_set_pos(ringind, 345, 3);
	lv_obj_set_size(mogoring1, 46, 78);
	lv_obj_set_size(mogoring2, 46, 78);
	lv_obj_set_size(mogoringback1, 46, 78);
	lv_obj_set_size(mogoringback2, 46, 78);
	lv_obj_set_size(alliancering, 66, 14);
	lv_obj_set_pos(mogoring1, 260, 149);
	lv_obj_set_pos(mogoring2, 341, 149);
	lv_obj_set_pos(mogoringback1, 260, 149);
	lv_obj_set_pos(mogoringback2, 341, 149);
	lv_obj_set_pos(alliancering, 267, 72);
	lv_obj_move_background(mogoring1);
	lv_obj_move_background(mogoring2);
	lv_obj_add_style(ringind, &styleind, LV_PART_MAIN);
	lv_obj_add_style(mogoring1, &styleind, LV_PART_MAIN);
	lv_obj_add_style(mogoring2, &styleind, LV_PART_MAIN);
	lv_obj_add_style(mogoringback1, &styleind, LV_PART_MAIN);
	lv_obj_add_style(mogoringback2, &styleind, LV_PART_MAIN);
	lv_obj_add_style(alliancering, &styleind, LV_PART_MAIN);
	lv_obj_set_scrollbar_mode(mogoringback1, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(mogoringback2, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(alliancering, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_style_pad_top(jauton, 8, LV_PART_ITEMS);
	lv_obj_set_style_pad_bottom(jauton, 6, LV_PART_ITEMS);
	lv_obj_set_style_pad_left(jauton, 4, LV_PART_ITEMS);
	lv_obj_add_style(jauton, &style, LV_PART_MAIN);
	lv_obj_add_style(jauton, &style, LV_PART_ITEMS);
	lv_table_set_col_width(jauton, 0, 230);
	lv_obj_move_background(jauton);
	lv_obj_add_style(selectedAuton, &style, LV_PART_MAIN);
	lv_obj_set_pos(selectedAuton, 5, 182);
	lv_obj_set_size(selectedAuton, 230, 47);
	lv_obj_add_style(selectedAutonbox, &style, LV_PART_MAIN);
	lv_obj_set_size(selectedAutonbox, 230, 230);
	lv_obj_set_pos(selectedAutonbox, 5, 5);
	lv_obj_move_background(selectedAutonbox);
	lv_obj_set_style_pad_left(selectedAuton, 4, LV_PART_MAIN);
	lv_obj_set_style_pad_top(selectedAuton, 8, LV_PART_MAIN);
	lv_label_set_text(selectedAuton, "No auton selected");
	noselection = true;
	lv_obj_set_size(jauton, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	lv_obj_set_pos(jauton, 5, 60);
	lv_obj_move_foreground(autonselectup);
	lv_obj_move_foreground(autonselectdown);
	lv_obj_add_style(redblu, &styleswitch, LV_PART_INDICATOR | LV_STATE_DEFAULT); 
	lv_obj_add_style(redblu, &styleswitch, LV_PART_INDICATOR | LV_STATE_CHECKED);
	lv_obj_add_style(redblu, &styleswitch, LV_PART_KNOB);
	lv_obj_set_style_pad_all(redblu, 0, LV_PART_KNOB);
	lv_obj_set_style_pad_hor(redblu, 2, LV_PART_KNOB);
	lv_obj_set_style_bg_color(redblu, lv_color_hex(0xff2a00), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(redblu, lv_color_hex(0x0066cc), LV_PART_INDICATOR | LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(redblu, lv_color_hex(0xcfffe9), LV_PART_KNOB);
	lv_obj_set_size(redblu, 42, 20);
	lv_obj_set_pos(redblu, 62, 10);
	lv_obj_move_foreground(redblu);
	lv_obj_add_style(posneg, &styleswitch, LV_PART_INDICATOR | LV_STATE_DEFAULT);
	lv_obj_add_style(posneg, &styleswitch, LV_PART_INDICATOR |LV_STATE_CHECKED);
	lv_obj_add_style(posneg, &styleswitch, LV_PART_KNOB);
	lv_obj_set_style_pad_all(posneg, 0, LV_PART_KNOB);
	lv_obj_set_style_pad_hor(posneg, 2, LV_PART_KNOB);
	lv_obj_set_style_bg_color(posneg, lv_color_hex(0x00b5bc), LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(posneg, lv_color_hex(0xdb8826), LV_PART_INDICATOR | LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(posneg, lv_color_hex(0xcfffe9), LV_PART_KNOB);
	lv_obj_set_size(posneg, 42, 20);
	lv_obj_set_pos(posneg, 177, 10);
	lv_obj_move_foreground(posneg);
	lv_obj_set_size(negind, 66, 30);
	lv_obj_set_pos(negind, 412, 197);
	lv_obj_set_scrollbar_mode(negind, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_style(negind, &styleind, LV_PART_MAIN);
	lv_obj_set_size(posind, 66, 66);
	lv_obj_set_pos(posind, 412, 126);
	lv_obj_add_style(posind, &styleind, LV_PART_MAIN);
	lv_obj_set_size(redbluind, 62, 79);
	lv_obj_set_pos(redbluind, 266, 22);
	lv_obj_add_style(redbluind, &styleind, LV_PART_MAIN);
	lv_obj_set_pos(autondesc, 7, 45);
	lv_obj_add_style(autondesc, &style, LV_PART_MAIN);
	lv_label_set_text(autondesc, "auton selection:");

	lv_obj_add_event_cb(redblu, jautonCurate, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(posneg, jautonCurate, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(autonselectup, upDownBtn, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(autonselectdown, upDownBtn, LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(jauton, selectAuton, LV_EVENT_VALUE_CHANGED, NULL);
	lv_event_send(redblu, LV_EVENT_CLICKED, NULL);

	tempcheck();
	autonbuilderinit();
	manualbuilderinit();

	for(int i = 0; i < 4; i++) {
		pageswitch = lv_img_create(screens[i % 3]);
		lv_img_set_src(pageswitch, i < 3 ? LV_SYMBOL_RIGHT : LV_SYMBOL_LEFT);
		lv_obj_set_size(pageswitch, 18, 18);
		lv_obj_set_pos(pageswitch, i < 3 ? 456 : 6, 6);
		lv_obj_add_style(pageswitch, &style, LV_PART_MAIN);
		lv_obj_add_flag(pageswitch, LV_OBJ_FLAG_CLICKABLE);
		lv_obj_set_style_text_opa(pageswitch, 128, LV_STATE_PRESSED);
		lv_obj_move_foreground(pageswitch);
		lv_obj_add_event_cb(pageswitch, pageSwitchBtn, LV_EVENT_CLICKED, &pageside[i < 3 ? 0 : 1]);
		cout << i << endl;
	}
}

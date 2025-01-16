#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

void screeninit();
void jautonrun();

extern lv_obj_t *mainscreen;
extern lv_obj_t* autoselector;
extern lv_obj_t* motortemps;
extern lv_obj_t* autobuilder;
extern lv_obj_t* manbuilder;
extern lv_obj_t* pageswitch;
extern lv_obj_t* ringind;

extern vector<lv_color32_t> colortable;
extern bool noselection;
extern int scrpage;

namespace jas {
class jasauton {
   public:
	jasauton();
	jasauton(std::function<void()> autoncall, int redblufilt, int posnegfilt, std::string name, std::string desc, int mogo1, int mogo2, bool allyring);
	std::function<void()> AutonCall;
	int RedBluFilt;
	int PosNegFilt;
	std::string Name;
	std::string Desc;
	int Mogo1;
	int Mogo2;
	bool AllyRing;
};

class jautontable {
   public:
	std::vector<jasauton> jasautontable;
	void jautonpopulate(std::vector<jasauton> JasAutonTable);
};
}  // namespace jas
extern jas::jautontable j_auton_selector;
extern vector<jas::jasauton> jautoncurated;
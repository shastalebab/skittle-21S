#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

void tempcheck();
void tempcheckctrl();

namespace jas {
class motors {
   public:
	struct motordata {
		pros::Motor motor;
		std::string name;
	};
};
}

extern vector<jas::motors::motordata> motorbar;
extern vector<lv_obj_t *> motorboxes;
extern lv_obj_t *mainlabel;
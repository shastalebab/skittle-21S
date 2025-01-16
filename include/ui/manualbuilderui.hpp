#pragma once

#include "EZ-Template/api.hpp"  // IWYU pragma: keep
#include "api.h"  // IWYU pragma: keep

void manualbuilderinit();
void mancallback();

extern int color;
extern int startposition;
extern bool positionorient;
extern bool activescreen;
extern lv_obj_t *allianceslider;
extern pros::task_t ringsorting;

namespace jas {
    class autobuildermodules {
        public:
        autobuildermodules();
        autobuildermodules(std::string name, std::string desc);
        autobuildermodules(std::string name, std::string desc, vector<pair<int, int>> points);
        std::string Name;
        std::string Desc;
        vector<pair<int, int>> Points;
    };
}
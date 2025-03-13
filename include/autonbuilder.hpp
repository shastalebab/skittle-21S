#pragma once

#include "EZ-Template/drive/drive.hpp"
#include "subsystems_auton.hpp"

extern Drive chassis;
extern Coordinate currentPoint;

void alliancestake();

void blue_goal_rush();
void red_goal_rush();
void blue_gr_mogo();
void red_gr_mogo();
void left_mogo();
void right_mogo();

void left_corner();
void right_corner();

void red_ring_rush();
void blue_ring_rush();

void left_btm_ring();
void mid_top_ring();
void right_btm_ring();

void ladder();
void drop_mogo();
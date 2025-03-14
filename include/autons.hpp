#pragma once

#include "EZ-Template/drive/drive.hpp"

extern Drive chassis;

extern bool cornerState;

void testautonRed();
void testautonBlue();

void red_4pos();
void red_6ring();
void red_negsolowp();

void blue_4pos();
void blue_gr_wp();
void blue_6ring();
void blue_negsolowp();

void move_forward();

void default_constants();
void ram();
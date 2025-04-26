#pragma once
#include "Crystalfontz128x128_ST7735.h"

extern void init_display();

extern void display_information(int temp);

extern void display_list(char* menuItems);

extern void select_index();

extern void scroll_down();

extern void scroll_up();

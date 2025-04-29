#pragma once
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

extern void init_display(Graphics_Context *g_sContext);

extern void display_information(float temp, Graphics_Context *g_sContext);

extern void display_list(char menuItems[][20], Graphics_Context *g_sContext);

extern void select_index(int index, char* menuItems[20], Graphics_Context *g_sContext);

extern void scroll_down(char** menuItems, Graphics_Context *g_sContext);

extern void scroll_up(char** menuItems, Graphics_Context *g_sContext);

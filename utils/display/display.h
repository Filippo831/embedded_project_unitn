#pragma once
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "stdio.h"

extern void init_display(Graphics_Context *g_sContext);

extern void display_information(float temp, Graphics_Context *g_sContext);

extern void get_list(char list[][20]);

extern void display_list(Graphics_Context *g_sContext);

extern void select_index(int index, Graphics_Context *g_sContext);

extern void scroll_down( Graphics_Context *g_sContext);

extern void scroll_up(Graphics_Context *g_sContext);

#pragma once
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "stdio.h"

// init display
extern void init_display(Graphics_Context *g_sContext, uint8_t *index);

// display navbar with temperature value
extern void display_information(float temp, Graphics_Context *g_sContext);

// pass string list from main
extern void get_list(char list[][20]);

// refresh the display displaying the list
extern void display_list(Graphics_Context *g_sContext);

// select a specified index
extern void select_index(int index, Graphics_Context *g_sContext);

// highlight the element underneath the current one.
// If the previous highlighted is the last displayed and there are other
// elements, scroll the list down 1 element
extern void scroll_down( Graphics_Context *g_sContext);

// highlight the element over the current one.
// If the previous highlighted is the first displayed and there are other
// elements on top, scroll the list up 1 element
extern void scroll_up(Graphics_Context *g_sContext);

// return the selected element string
extern char* get_element();

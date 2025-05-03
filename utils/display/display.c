#include "display.h"


#define MAX_VISIBLE_ITEMS 5

#define NAV_HEIGHT 27
#define BODY_HEIGHT 100


void init_display(Graphics_Context *g_sContext) {
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
}


/* display navbar with current temperpature
 * make the sensore push a read every now and then, use a timer for this
 *
 */
void display_information(float temp, Graphics_Context *g_sContext) {
    char tempString[10];
    sprintf(tempString, "%.1fC", temp);
    
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawStringCentered(g_sContext, (int8_t *)tempString, AUTO_STRING_LENGTH, 64, 12, OPAQUE_TEXT);
}

// contains the index of action selected, used to highlight the right row
int selectedIndex = 0;

// take count of how many items are on top of the highest displayed string
int scrollOffset = 0;

void display_list(char menuItems[][20], Graphics_Context *g_sContext) {
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_SANDY_BROWN);
    Graphics_Rectangle listAreaRect = {0, 127-BODY_HEIGHT, 127, 127};
    Graphics_fillRectangle(g_sContext, &listAreaRect);
    
    int i;
    for (i = 0; i < 3; i++) {
        int itemIndex = i + scrollOffset;
        if (itemIndex >= 3)
            break;
        
        if (itemIndex == selectedIndex) {
            // Highlighted background
            Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BROWN);
            Graphics_Rectangle itemRect = {0, NAV_HEIGHT + (20*i) + 3, 127, NAV_HEIGHT + (20*i) + 15};
            Graphics_fillRectangle(g_sContext, &itemRect);
            Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
        } else {
            Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
        }
        

        Graphics_drawString(g_sContext, (int8_t *)menuItems[itemIndex], AUTO_STRING_LENGTH, 10, 33 + (i * 20), OPAQUE_TEXT);
    }
}


void select_index(int index, char menuItems[][20], Graphics_Context *g_sContext) {
    selectedIndex = index;
    display_list(menuItems, g_sContext);
}

void scroll_up(char** menuItems, Graphics_Context *g_sContext) {
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < scrollOffset) {
            scrollOffset--;
        }
        display_list(menuItems, g_sContext);
    }
}

void scroll_down(char** menuItems, Graphics_Context *g_sContext) {
    int totalItems = 3;
    if (selectedIndex < totalItems - 1) {
        selectedIndex++;
        if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
            scrollOffset++;
        }
        display_list(menuItems, g_sContext);
    }
}

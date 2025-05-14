#include "display.h"


#define MAX_VISIBLE_ITEMS 5

#define NAV_HEIGHT 27
#define BODY_HEIGHT 100

static char (*menuItems)[20];
static int totalItems;

// contains the index of action selected, used to highlight the right row
uint8_t selectedIndex = 0;

void init_display(Graphics_Context *g_sContext, uint8_t *index) {
    index = &selectedIndex;
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setBackgroundColor(g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(g_sContext);
}

void get_list(char list[][20]) {
    menuItems = list;

    int index = 0;
    while (1) {
        if (list[index][0] == 0) {
            break;
        }
        index = index + 1;
    }

    totalItems = index;
}

/* display navbar with current temperpature
 * make the sensore push a read every now and then, use a timer for this
 *
 */
void display_information(float temp, Graphics_Context *g_sContext) {
    char tempString[10];
    sprintf(tempString, "%.1fC", temp);
    
    Graphics_drawStringCentered(g_sContext, (int8_t *)tempString, AUTO_STRING_LENGTH, 64, 12, OPAQUE_TEXT);

    Graphics_Rectangle listAreaRect = {0, NAV_HEIGHT-2, 127, NAV_HEIGHT};
    Graphics_fillRectangle(g_sContext, &listAreaRect);

}



// take count of how many items are on top of the highest displayed string
int scrollOffset = 0;

void display_list(Graphics_Context *g_sContext) {
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_Rectangle listAreaRect = {0, 127-BODY_HEIGHT, 127, 127};
    Graphics_fillRectangle(g_sContext, &listAreaRect);
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);

    
    int i;

    for (i = 0; i < totalItems; i++) {
        int itemIndex = i + scrollOffset;
        if (itemIndex >= totalItems)
            break;
        
        if (itemIndex == selectedIndex) {
            // Highlighted background
            Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BROWN);
            Graphics_Rectangle itemRect = {0, NAV_HEIGHT + (20*i) + 3, 127, NAV_HEIGHT + (20*i) + 15};
            Graphics_fillRectangle(g_sContext, &itemRect);
            Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
        }

        Graphics_drawString(g_sContext, (int8_t *)menuItems[itemIndex], AUTO_STRING_LENGTH, 10, 33 + (i * 20), OPAQUE_TEXT);
    }
}


void select_index(int index, Graphics_Context *g_sContext) {
    selectedIndex = index;
    display_list(g_sContext);
}

void scroll_up(Graphics_Context *g_sContext) {
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < scrollOffset) {
            scrollOffset--;
        }
        display_list(g_sContext);
    }
}

void scroll_down(Graphics_Context *g_sContext) {
    if (selectedIndex < totalItems - 1) {
        selectedIndex++;
        if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
            scrollOffset++;
        }
        display_list(g_sContext);
    }
}

char *get_element() {
    return menuItems[selectedIndex];
}

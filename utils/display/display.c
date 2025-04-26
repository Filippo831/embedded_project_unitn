#include "display.h"

#define MAX_VISIBLE_ITEMS 5

void init_display() {
    // Initialize the display
    Crystalfontz128x128_Init();

    // Set orientation if needed (optional)
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize graphics context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
}

void display_information(int temp) {
    char tempString[10];
    sprintf(tempString, "%.1fC", temp);
    
    Graphics_setForegroundColor(&g_sContext, COLOR_WHITE);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)tempString, AUTO_STRING_LENGTH, 64, 8, OPAQUE);
}

int selectedIndex = 0; // current selected item
int scrollOffset = 0;  // top item index

void display_list(char* menuItems) {
    Graphics_setForegroundColor(&g_sContext, COLOR_BLACK);
    Graphics_fillRectangle(&g_sContext, &listAreaRect); // Clear the list area first
    
    for (int i = 0; i < MAX_VISIBLE_ITEMS; i++) {
        int itemIndex = i + scrollOffset;
        if (itemIndex >= sizeof(menuItems) / sizeof(menuItems[0]))
            break;
        
        if (itemIndex == selectedIndex) {
            // Highlighted background
            Graphics_setForegroundColor(&g_sContext, COLOR_RED);
            Graphics_fillRectangle(&g_sContext, &itemRect); // Calculate based on i
            Graphics_setForegroundColor(&g_sContext, COLOR_WHITE);
        } else {
            Graphics_setForegroundColor(&g_sContext, COLOR_WHITE);
        }
        
        // make the size dynamic based on the max amount of item to display at the same time
        Graphics_drawString(&g_sContext, (int8_t *)menuItems[itemIndex], AUTO_STRING_LENGTH, 10, 20 + (i * 20), OPAQUE);
    }
}


void select_index(int index) {
    if (index >= 0 && index < sizeof(menuItems)/sizeof(menuItems[0])) {
        selectedIndex = index;
        display_list();
    }
}

void scroll_up() {
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < scrollOffset) {
            scrollOffset--;
        }
        display_list();
    }
}

void scroll_down() {
    int totalItems = sizeof(menuItems) / sizeof(menuItems[0]);
    if (selectedIndex < totalItems - 1) {
        selectedIndex++;
        if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
            scrollOffset++;
        }
        display_list();
    }
}

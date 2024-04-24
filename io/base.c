#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <bcm2835.h>

#include "lib/buttons.h"
#include "lib/colors.h"
#include "lib/device.h"
#include "lib/display.h"
#include "lib/fonts/fonts.h"
#include "lib/log.h"

#define VIEWER_FOLDER "viewer/"
#define MAX_ENTRIES 100
#define MAX_FILE_NAME 255
#define MAX_TEXT_SIZE 400

// Colors — Feel free to change these to fit your preference
#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

// Makes sure to deinitialize everything before program close
void intHandler(int dummy) {
    log_info("Exiting...");
    display_exit();
    exit(0);
}

int get_entries(char *folder, char entries[][MAX_FILE_NAME]) {}

void draw_menu(char entries[][MAX_FILE_NAME], int num_entries, int selected) {}

void draw_file(char *folder, char *file_name) {}

int main(void) {
    signal(SIGINT, intHandler);

    log_info("Starting...");

    char entries[MAX_ENTRIES][MAX_FILE_NAME];
    // Initialize the hardware
    if (DEV_ModuleInit() != 0) {
        log_info("Device initialization failed!");
        return -1;
    }

    // Get directory contents
    int entry_count = get_entries(VIEWER_FOLDER, entries);

    // Initially draw menu
    int current_selection = 0;
    draw_menu(entries, entry_count, current_selection);

    while (true) {
        delay_ms(200);

        // Button logic for navigation
        if (button_up()) {
            current_selection = (current_selection - 1 + entry_count) % entry_count;
            draw_menu(entries, entry_count, current_selection);
        }

        if (button_down()) {
            current_selection = (current_selection + 1) % entry_count;
            draw_menu(entries, entry_count, current_selection);
        }

        if (button_center()) {
            log_info("Selected: %s", entries[current_selection]);
            draw_file(VIEWER_FOLDER, entries[current_selection]);
        }
    }

    return 0;
}

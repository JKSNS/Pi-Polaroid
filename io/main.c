#include "lib/buttons.h"
#include "lib/colors.h"
#include "lib/device.h"
#include "lib/display.h"
#include "lib/fonts/fonts.h"
#include "lib/log.h"
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Definitions
#define VIEWER_FOLDER "viewer/"
#define MAX_ENTRIES 8
#define MAX_FILE_NAME 100
#define MAX_TEXT_SIZE 400

// Colors — Feel free to change these to fit your preference
#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

// Signal handler for interrupt (SIGINT)
void intHandler(int dummy) {
    log_info("Exiting...");
    display_exit(); // Exit display
    exit(0);        // Exit program
}

// Function to get file entries in a directory
int get_entries(char *folder, char entries[][MAX_FILE_NAME]) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(folder); // Open directory
    if (dir == NULL) {
        log_error("Could not open directory %s", folder);
        return -1; // Return error if directory cannot be opened
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Filter files by extension (.bmp or .log)
        if (strstr(entry->d_name, ".bmp") != NULL || strstr(entry->d_name, ".log") != NULL) {
            strcpy(entries[count], entry->d_name); // Copy file name
            count++;
            if (count >= MAX_ENTRIES) {
                break;
            }
        }
    }

    closedir(dir); // Close directory
    return count;  // Return number of entries found
}

// Function to draw the menu on display
void draw_menu(char entries[][MAX_FILE_NAME], int num_entries, int selected) {
    display_clear(BACKGROUND_COLOR); // Clear display

    // Iterate through entries and draw them on display
    for (int i = 0; i < num_entries && i < MAX_ENTRIES; i++) {
        if (i == selected) {
            // Highlight selected entry
            display_draw_string(0, i * 16, entries[i], &Font8, SELECTED_BG_COLOR,
                                SELECTED_FONT_COLOR);
        } else {
            // Draw non-selected entries
            display_draw_string(0, i * 16, entries[i], &Font8, BACKGROUND_COLOR, FONT_COLOR);
        }
    }
}

// Function to display file content or image
void draw_file(char *folder, char *file_name) {
    char file_path[MAX_FILE_NAME];
    snprintf(file_path, MAX_FILE_NAME, "%s%s", folder,
             file_name); // Concatenate folder and file name

    // Check file extension and draw accordingly
    if (strstr(file_name, ".bmp") != NULL) {
        display_draw_image(file_path); // Display image
    } else if (strstr(file_name, ".log") != NULL) {
        FILE *fp = fopen(file_path, "r"); // Open file
        if (fp != NULL) {
            char text[MAX_TEXT_SIZE];
            // Read file content
            if (fread(text, 1, MAX_TEXT_SIZE - 1, fp) > 0) {
                text[MAX_TEXT_SIZE - 1] = '\0'; // Null-terminate string
                display_draw_string(0, 0, text, &Font8, BACKGROUND_COLOR,
                                    FONT_COLOR); // Display text
            }
            fclose(fp); // Close file
        } else {
            log_error("Could not open file %s", file_path); // Log error if file cannot be opened
        }
    }

    delay_ms(2000); // Display for 2 seconds
}

// Main function
int main(void) {
    signal(SIGINT, intHandler); // Register interrupt handler
    log_info("Starting...");    // Log start of program

    display_init();
    buttons_init();

    char entries[MAX_ENTRIES][MAX_FILE_NAME]; // Array to store file entries
    int num_entries = 0;                      // Number of entries
    int selected = 0;                         // Currently selected entry

    // DEV_ModuleInit(); // Initialize hardware
    // display_init(); // Initialize display

    // Get file entries from directory
    num_entries = get_entries(VIEWER_FOLDER, entries);
    if (num_entries < 0) {
        log_error("Error getting entries"); // Log error if entries cannot be retrieved
        return -1;                          // Exit with error
    }

    draw_menu(entries, num_entries, selected); // Draw initial menu

    while (true) {
        delay_ms(200); // Delay to debounce buttons

        // Check if up button is pressed
        if (button_up() == 0) {
            while (button_up() == 0) {
                delay_ms(1); // Wait for button release
            }
            selected = (selected - 1 + num_entries) % num_entries; // Move selection up
            printf("%d", selected);
            draw_menu(entries, num_entries, selected); // Redraw menu
        }

        // Check if down button is pressed
        if (button_down() == 0) {
            while (button_down() == 0) {
                delay_ms(1); // Wait for button release
            }
            selected = (selected + 1) % num_entries; // Move selection down
            printf("%d", selected);
            draw_menu(entries, num_entries, selected); // Redraw menu
        }

        // Check if center button is pressed
        if (button_center() == 0) {
            while (button_center() == 0) {
                delay_ms(1); // Wait for button release
            }
            display_clear(BACKGROUND_COLOR);
            draw_file(VIEWER_FOLDER, entries[selected]); // Display file content or image
            draw_menu(entries, num_entries, selected);   // Redraw menu
        }
    }

    return 0;
}

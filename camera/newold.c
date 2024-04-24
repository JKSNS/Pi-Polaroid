#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/buttons.h"
#include "lib/camera.h"
#include "lib/colors.h"
#include "lib/device.h"
#include "lib/display.h"
#include "lib/fonts/fonts.h"
#include "lib/image.h"
#include "lib/log.h"

#include <stdbool.h>

#define VIEWER_FOLDER "viewer/"
#define MAX_ENTRIES 8
#define MAX_TEXT_SIZE 400
#define MAX_FILE_NAME 100

// Colors — Feel free to change these to fit your preference
#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

// Global variables
char entries[MAX_ENTRIES][MAX_FILE_NAME];
int num_entries = 0, selected = 0;
bool imageDisplayed = false; // Tracks if an image is currently displayed
Bitmap bmp;                  // Used for image manipulation

// Makes sure to deinitialize everything before program close
void intHandler(int dummy) {
    log_info("Exiting...");
    display_exit();
    exit(0);
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

// Function declarations
void capture_and_display_image();
void update_menu();
void apply_filter_and_display(Color color);
void or_filter_and_display();
void cleanup_and_return_to_menu();

void capture_and_display_image() {
    uint8_t buf[IMG_SIZE];
    camera_capture_data(buf, IMG_SIZE);                               // Capture image data
    camera_save_to_file(buf, IMG_SIZE, VIEWER_FOLDER "doorbell.bmp"); // Save image
    create_bmp(&bmp, buf); // Convert raw data to Bitmap struct for display
    // Display image directly or flag for filtering/display
    imageDisplayed = true;
}

void update_menu() {
    // Re-read the directory to update the menu entries
    num_entries = get_entries(VIEWER_FOLDER, entries);
    draw_menu(entries, num_entries, selected);
}

void apply_filter_and_display(Color color) {
    remove_color_channel(color, &bmp); // Apply color filter
    // Assuming function to draw Bitmap directly or using bmp.pxl_data
}

void or_filter_and_display() {
    or_filter(&bmp); // Apply OR filter
    // Draw the modified image
}

void cleanup_and_return_to_menu() {
    destroy_bmp(&bmp);      // Clean up Bitmap resources
    imageDisplayed = false; // Reset flag
    update_menu();          // Refresh and display the menu
}

int main(void) {
    // Initial setup
    signal(SIGINT, intHandler);
    display_init();
    buttons_init();
    num_entries = get_entries(VIEWER_FOLDER, entries);
    draw_menu(entries, num_entries, selected);

    while (true) {
        delay_ms(200); // Debounce delay

        // If no image is displayed, navigate the menu
        if (!imageDisplayed) {
            // Right button to display selected file
            if (button_right() == 0) {
                while (button_right() == 0) {
                    delay_ms(1); // Wait for release
                }
                // Display the selected file and set the imageDisplayed flag
                draw_file(VIEWER_FOLDER, entries[selected]);
                imageDisplayed = true;
            }

            // Center button to capture and display image
            if (button_center() == 0) {
                while (button_center() == 0) {
                    delay_ms(1); // Wait for release
                }
                capture_and_display_image();
            }
        } else {
            // Image display mode: apply filters or return to menu
            if (button_right() == 0) {
                apply_filter_and_display(RED_CHANNEL);
            } else if (button_left() == 0) {
                apply_filter_and_display(BLUE_CHANNEL);
            } else if (button_up() == 0) {
                apply_filter_and_display(GREEN_CHANNEL);
            } else if (button_down() == 0) {
                or_filter_and_display();
            } else if (button_center() == 0) {
                cleanup_and_return_to_menu();
            }
        }
    }

    return 0;
}
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

uint8_t imageBuffer[IMG_SIZE];

// Remove a specific color channel from the image
void remove_color_channel(Color color, Bitmap *bmp) {
    for (uint32_t i = 0; i < bmp->pxl_data_size; i += 3) {
        switch (color) {
        case BLUE_CHANNEL:        // Intended to remove blue
            bmp->pxl_data[i] = 0; // Correct index for blue in BGR
            break;
        case GREEN_CHANNEL:           // Intended to remove green
            bmp->pxl_data[i + 1] = 0; // Correct index for green in BGR
            break;
        case RED_CHANNEL:             // Intended to remove red
            bmp->pxl_data[i + 2] = 0; // Correct index for red in BGR
            break;
        }
    }
}

// Apply an OR filter to the image
void or_filter(Bitmap *bmp) {
    // Allocate a temporary buffer for storing the modified pixel data
    uint8_t *temp_data = malloc(bmp->pxl_data_size);
    if (!temp_data) {
        fprintf(stderr, "Memory allocation failed for OR filter operation.\n");
        return;
    }

    // Copy the original pixel data to the temporary buffer
    memcpy(temp_data, bmp->pxl_data, bmp->pxl_data_size);

    // Process each pixel except for those in the first and last rows
    for (uint32_t y = 1; y < bmp->img_height - 1; y++) {
        for (uint32_t x = 0; x < bmp->img_width; x++) {
            uint32_t index = (y * bmp->img_width + x) * 3; // Index for the current pixel
            uint32_t above = index - bmp->img_width * 3;   // Index for the pixel above
            uint32_t below = index + bmp->img_width * 3;   // Index for the pixel below

            // Apply the OR filter on each color channel with the pixels directly above and below
            for (int color = 0; color < 3; color++) {
                temp_data[index + color] = bmp->pxl_data[index + color] |
                                           bmp->pxl_data[above + color] |
                                           bmp->pxl_data[below + color];
            }
        }
    }

    // Copy the modified pixel data back to the original data buffer
    memcpy(bmp->pxl_data, temp_data, bmp->pxl_data_size);

    // Free the temporary buffer
    free(temp_data);
}

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

// Declaration (if the function is defined after its use in main)
void redraw_bitmap(Bitmap *bmp);

void redraw_bitmap(Bitmap *bmp) {
    if (bmp != NULL && bmp->pxl_data != NULL) {
        // Make sure the BMP file has been correctly opened and processed before this point.
        display_draw_image_data(bmp->pxl_data, bmp->img_width, bmp->img_height);
    } else {
        fprintf(stderr, "Failed to load or process BMP file.\n");
    }
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
    bool viewingImage = false;                // Flag to indicate if an image is being viewed

    Bitmap *currentImage = NULL; // To hold the current image being viewed/processed

    display_init(); // Initialize display

    // Get file entries from directory
    num_entries = get_entries(VIEWER_FOLDER, entries);
    if (num_entries < 0) {
        log_error("Error getting entries");
        return -1; // Exit with error
    }

    draw_menu(entries, num_entries, selected); // Draw initial menu
    while (true) {
        delay_ms(200); // Delay to debounce buttons

        if (viewingImage) {
            // Check if right button is pressed for RED channel removal
            if (button_right() == 0) {
                while (button_right() == 0) {
                    delay_ms(1); // Debounce button press
                }
                if (currentImage != NULL) {
                    remove_color_channel(RED_CHANNEL, currentImage);
                    display_clear(BACKGROUND_COLOR);
                    // Assuming a function to redraw the bitmap
                    redraw_bitmap(currentImage);
                }
            }
            // Check if left button is pressed for BLUE channel removal
            else if (button_left() == 0) {
                while (button_left() == 0) {
                    delay_ms(1); // Debounce button press
                }
                if (currentImage != NULL) {
                    remove_color_channel(BLUE_CHANNEL, currentImage);
                    display_clear(BACKGROUND_COLOR);
                    redraw_bitmap(currentImage);
                }
            }
            // Check if up button is pressed for GREEN channel removal
            else if (button_up() == 0) {
                while (button_up() == 0) {
                    delay_ms(1); // Debounce button press
                }
                if (currentImage != NULL) {
                    remove_color_channel(GREEN_CHANNEL, currentImage);
                    display_clear(BACKGROUND_COLOR);
                    redraw_bitmap(currentImage);
                }
            } else if (button_down() == 0) {
                while (button_down() == 0) {
                    delay_ms(1); // Debounce button press
                }
                if (currentImage != NULL) {
                    log_info("testing");
                    or_filter(currentImage);
                    display_clear(BACKGROUND_COLOR);
                    redraw_bitmap(currentImage);
                }
            }
            // Check if center button is pressed to exit viewing mode
            else if (button_center() == 0) {
                while (button_center() == 0) {
                    delay_ms(1); // Debounce button press
                }
                viewingImage = false;
                display_clear(BACKGROUND_COLOR);
                draw_menu(entries, num_entries, selected);
                if (currentImage != NULL) {
                    destroy_bmp(currentImage);
                    currentImage = NULL;
                }
            }
            continue; // Skip the rest of the loop when viewing an image
        }
        // take picture, save it, and display to screen.
        if (!viewingImage) {
            // Menu navigation
            if (button_up() == 0) {
                while (button_up() == 0) {
                    delay_ms(1);
                }
                selected = (selected - 1 + num_entries) % num_entries;
                draw_menu(entries, num_entries, selected);
            } else if (button_down() == 0) {
                while (button_down() == 0) {
                    delay_ms(1);
                }
                selected = (selected + 1) % num_entries;
                draw_menu(entries, num_entries, selected);
            } else if (button_right() == 0) {
                while (button_right() == 0) {
                    delay_ms(1);
                }
                viewingImage = true;
                display_clear(BACKGROUND_COLOR);
                draw_file(VIEWER_FOLDER, entries[selected]); // Show the selected file
                // No need to redraw the menu here, as we're viewing the image
            } else if (button_center() == 0) {
                while (button_center() == 0) {
                    delay_ms(1); // Debounce button press
                }

                // Capture the image into the buffer
                camera_capture_data(imageBuffer, IMG_SIZE);

                // Construct the filename for the new image
                char newImageFilename[MAX_FILE_NAME];
                snprintf(newImageFilename, MAX_FILE_NAME, "%s/new_image.bmp", VIEWER_FOLDER);

                // Save the captured image to a file
                camera_save_to_file(imageBuffer, IMG_SIZE, newImageFilename);

                // bitmap struct... , big text description.

                // another while loop that does the image filtering instead of the if else

                // statements below. then once you press center again, redo the loop...

                // Refresh the entries list to include the new image
                num_entries = get_entries(VIEWER_FOLDER, entries);

                // Redraw the menu to show the new image entry
                draw_menu(entries, num_entries, selected);
            }

        } else {
            // Check if down button is pressed for OR filter application
            if (button_down() == 0) {
                while (button_down() == 0) {
                    delay_ms(1); // Debounce button press
                }

                if (currentImage != NULL && viewingImage) {
                    or_filter(currentImage);
                    display_clear(BACKGROUND_COLOR); // Clear the display before redrawing
                    redraw_bitmap(currentImage);     // Redraw the bitmap with the filter applied
                }
            }

            // If viewing an image and button_center is pressed, return to menu
            if (button_center() == 0) {
                while (button_center() == 0) {
                    delay_ms(1);
                }
                viewingImage = false;
                display_clear(BACKGROUND_COLOR);
                draw_menu(entries, num_entries, selected);
            }
        }
    }

    return 0;
}

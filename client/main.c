#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/buttons.h"
#include "lib/camera.h"
#include "lib/client.h"
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

int centerX = DISPLAY_WIDTH / 2;
int centerY = DISPLAY_HEIGHT / 2;
// uint8_t imageBuffer[IMG_SIZE]; // Buffer to store the captured image
//  Bitmap currentImage;
Bitmap *load_bitmap(const char *filename);

Bitmap *load_bitmap(const char *filename) {
    // Allocate memory for the Bitmap struct
    Bitmap *bmp = malloc(sizeof(Bitmap));
    if (bmp == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        // Handle file opening failure
        free(bmp);
        return NULL;
    }

    return bmp;
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

    // Bitmap *currentImage = NULL; // To hold the current image being viewed/processed

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

                display_clear(BACKGROUND_COLOR);
                draw_file(VIEWER_FOLDER, entries[selected]); // Show the selected file
                // No need to redraw the menu here, as we're viewing the image
                delay_ms(2000);
                display_clear(BACKGROUND_COLOR); // test
                draw_menu(entries, num_entries, selected);

            } else if (button_center() == 0) {
                while (button_center() == 0) {
                    delay_ms(1); // Debounce button press
                }

                uint8_t *imageBuffer = malloc(sizeof(uint8_t) * IMG_SIZE);

                display_clear(WHITE);
                display_draw_string(centerX - 30, centerY, "Say Cheese!", &Font8, WHITE,
                                    FONT_COLOR);
                delay_ms(1000); // Give some time for the message to be read
                viewingImage = true;
                // Capture the image into the buffer
                camera_capture_data(imageBuffer, IMG_SIZE);

                // Construct the filename for the new image
                char newImageFilename[MAX_FILE_NAME];
                snprintf(newImageFilename, MAX_FILE_NAME, "%sBM.bmp", VIEWER_FOLDER);

                // Save the captured image to a file
                camera_save_to_file(imageBuffer, IMG_SIZE, newImageFilename);
                Bitmap bmp;
                create_bmp(&bmp, imageBuffer);
                // free(imageBuffer);

                // Directly draw the captured image
                // draw_file(VIEWER_FOLDER, "new_image.bmp");
                viewingImage = true; // Indicate that you're now in image viewing mode
                display_draw_image_data(bmp.pxl_data, 128, 128);

                while (viewingImage) {
                    // printf('test');
                    if (button_right() == 0) {
                        while (button_right() == 0) {
                            delay_ms(1); // Debounce button press
                        }
                        // printf('buttonright');
                        remove_color_channel(RED_CHANNEL, &bmp);
                        display_draw_image_data(bmp.pxl_data, 128, 128);
                        reset_pixel_data(&bmp);
                    }

                    // Apply Blue Channel Removal Filter
                    if (button_left() == 0) {
                        while (button_left() == 0) {
                            delay_ms(1); // Debounce button press
                        }
                        remove_color_channel(BLUE_CHANNEL, &bmp);
                        display_draw_image_data(bmp.pxl_data, 128, 128);
                        reset_pixel_data(&bmp);
                    }

                    // Apply Green Channel Removal Filter
                    if (button_up() == 0) {
                        while (button_up() == 0) {
                            delay_ms(1); // Debounce button press
                        }
                        remove_color_channel(GREEN_CHANNEL, &bmp);
                        display_draw_image_data(bmp.pxl_data, 128, 128);
                        reset_pixel_data(&bmp);
                    }

                    // Apply OR Filter
                    if (button_down() == 0) {
                        while (button_down() == 0) {
                            delay_ms(1); // Debounce button press
                        }
                        // if doesn't work, remove .pxldata
                        or_filter(&bmp);
                        display_draw_image_data(bmp.pxl_data, 128, 128);
                        reset_pixel_data(&bmp);
                    }
                    // Refresh the entries list to include the new image
                    if (button_center() == 0) {
                        while (button_center() == 0) {
                            delay_ms(1);
                        }
                        // sending image code
                        Config config = {.port = "2240",
                                         .host = "ecen224.byu.edu",
                                         .payload = imageBuffer,   // The image buffer with BMP data
                                         .payload_size = IMG_SIZE, // The size of the image data
                                         .hw_id = "E5A8EF0E5"};

                        int sockfd = client_connect(&config);
                        if (sockfd >= 0) {
                            client_send_image(sockfd, &config); // Send the image
                            client_receive_response(sockfd);    // Receive server response
                            client_close(sockfd);               // Close the connection
                        } else {
                            printf("Failed to connect to the server.\n");
                        }
                        // sending image code
                        //  free(imageBuffer);
                        destroy_bmp(&bmp);
                        num_entries = get_entries(VIEWER_FOLDER, entries);
                        selected =
                            num_entries - 1; // Optionally, automatically select the new image
                        viewingImage = false;
                        display_clear(BACKGROUND_COLOR); // test
                        num_entries = get_entries(VIEWER_FOLDER, entries);
                        draw_menu(entries, num_entries, selected);
                    }
                }
            }
        }
    }
    return 0;
}

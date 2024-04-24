#include <dirent.h>
#include <pthread.h>
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

int SEND_STATUS = 0;
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
    display_clear(BACKGROUND_COLOR);

    uint16_t x = 10;
    uint16_t y = 10;

    if (selected < 0) {
        selected = num_entries - 1;
    }
    if (selected > num_entries - 1) {
        selected = 0;
    }

    for (int i = 0; i < num_entries; i++) {
        display_draw_string(x, y, entries[i], &Font8, BACKGROUND_COLOR, FONT_COLOR);
        if (i == selected) {
            display_draw_rectangle(0, y - 5, 128, y + 12, SELECTED_BG_COLOR, true, 0);
            display_draw_string(x, y, entries[i], &Font8, SELECTED_BG_COLOR, SELECTED_FONT_COLOR);
        }
        y += 15;
    }

    uint16_t status_x = 10;
    uint16_t status_y = 116;
    if (SEND_STATUS == 1) {
        display_draw_rectangle(0, status_y - 5, 128, status_y + 12, CYAN2, true, 0);
        display_draw_string(status_x, status_y, "Sending...", &Font8, CYAN2, FONT_COLOR);
    } else if (SEND_STATUS == 2) {
        display_draw_rectangle(0, status_y - 5, 128, status_y + 12, GREEN2, true, 0);
        display_draw_string(status_x, status_y, "Done!", &Font8, GREEN2, FONT_COLOR);
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

void *send_image(void *buf) {
    SEND_STATUS = 1; // Indicate that sending has started
    uint8_t *imageBuffer =
        (uint8_t *)buf; // Cast void pointer to uint8_t pointer for the image data

    Config config = {
        .port = "2240",
        .host = "ecen224.byu.edu",
        .payload = imageBuffer,   // Use the passed buffer as the payload
        .payload_size = IMG_SIZE, // Specify the size of the image data
        .hw_id = "E5A8EF0E5"      // Your hardware ID
    };
    int sockfd = client_connect(&config);
    if (sockfd >= 0) {
        client_send_image(sockfd, &config); // Send the image
        client_receive_response(sockfd);    // Receive server response
        client_close(sockfd);               // Close the connection
        SEND_STATUS = 2;                    // Indicate success
    } else {
        printf("Failed to connect to the server.\n");
        SEND_STATUS = -1; // Indicate failure
    }

    // Optionally deal with the buffer if needed
    // free(imageBuffer); // Only free if you're sure it should be done here

    // use a delay for visual feedback, then reset the status?
    sleep(2);
    SEND_STATUS = 0;

    return NULL;
}

// Main function
int main(void) {
    signal(SIGINT, intHandler);
    log_info("Starting...");

    char entries[MAX_ENTRIES][MAX_FILE_NAME];
    display_init();
    buttons_init();
    int num_entries = get_entries(VIEWER_FOLDER, entries);
    int selected = 0;
    draw_menu(entries, num_entries, selected);

    while (true) {
        int current_status = SEND_STATUS;
        delay_ms(200);
        if (selected < 0) {
            selected = num_entries - 1;
        }
        if (selected > num_entries - 1) {
            selected = 0;
        }

        if (button_up() == 0) {
            draw_menu(entries, num_entries, --selected);
            while (button_up() == 0) {
                delay_ms(1);
            }
        }

        if (button_down() == 0) {
            draw_menu(entries, num_entries, ++selected);
            while (button_down() == 0) {
                delay_ms(1);
            }
        }

        if (button_right() == 0) {
            draw_file(VIEWER_FOLDER, entries[selected]);
            delay_ms(2000);
            draw_menu(entries, num_entries, selected);

            while (button_right() == 0) {
                delay_ms(1);
            }
        }

        if (button_center() == 0) {

            display_clear(BACKGROUND_COLOR);
            display_draw_string(10, 10, "Say cheese!", &Font8, BACKGROUND_COLOR, FONT_COLOR);

            uint32_t buffer_size = IMG_SIZE;
            uint8_t *my_new_buf = malloc(buffer_size);
            camera_capture_data(my_new_buf, buffer_size);
            camera_save_to_file(my_new_buf, buffer_size, "doorbell.bmp");

            Bitmap bmp;
            create_bmp(&bmp, my_new_buf);
            display_draw_image_data(bmp.pxl_data, 128, 128);

            while (true) {

                if (button_left() == 0) {
                    reset_pixel_data(&bmp);
                    remove_color_channel(BLUE_CHANNEL, &bmp);
                    while (button_left() == 0) {
                        delay_ms(1);
                    }
                }

                if (button_right() == 0) {
                    reset_pixel_data(&bmp);
                    remove_color_channel(RED_CHANNEL, &bmp);
                    while (button_right() == 0) {
                        delay_ms(1);
                    }
                }

                if (button_down() == 0) {
                    reset_pixel_data(&bmp);
                    or_filter(&bmp);
                    while (button_down() == 0) {
                        delay_ms(1);
                    }
                }

                if (button_up() == 0) {
                    reset_pixel_data(&bmp);
                    remove_color_channel(GREEN_CHANNEL, &bmp);
                    while (button_up() == 0) {
                        delay_ms(1);
                    }
                }

                if (button_center() == 0) {

                    pthread_t send_thread;
                    pthread_create(&send_thread, NULL, send_image, (void *)my_new_buf);

                    while (button_center() == 0) {
                        delay_ms(1);
                    }

                    break;
                }
                display_draw_image_data(bmp.pxl_data, 128, 128);
            }
            destroy_bmp(&bmp);
            num_entries = get_entries(VIEWER_FOLDER, entries);
            draw_menu(entries, num_entries, selected);

            while (button_center() == 0) {

                delay_ms(1);
            }
        }
        if (SEND_STATUS != current_status) {
            draw_menu(entries, num_entries, selected);
        }
    }

    return 0;
}

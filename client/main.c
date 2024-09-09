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

#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

uint8_t imageBuffer[IMG_SIZE];

void intHandler(int dummy) {
    log_info("Exiting...");
    display_exit();
    exit(0);
}

int get_entries(char *folder, char entries[][MAX_FILE_NAME]) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    dir = opendir(folder);
    if (dir == NULL) {
        log_error("Could not open directory %s", folder);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".bmp") != NULL || strstr(entry->d_name, ".log") != NULL) {
            strcpy(entries[count], entry->d_name);
            count++;
            if (count >= MAX_ENTRIES) {
                break;
            }
        }
    }

    closedir(dir);
    return count;
}

void draw_menu(char entries[][MAX_FILE_NAME], int num_entries, int selected) {
    display_clear(BACKGROUND_COLOR);

    for (int i = 0; i < num_entries && i < MAX_ENTRIES; i++) {
        if (i == selected) {
            display_draw_string(0, i * 16, entries[i], &Font8, SELECTED_BG_COLOR,
                                SELECTED_FONT_COLOR);
        } else {
            display_draw_string(0, i * 16, entries[i], &Font8, BACKGROUND_COLOR, FONT_COLOR);
        }
    }
}

void draw_file(char *folder, char *file_name) {
    char file_path[MAX_FILE_NAME];
    snprintf(file_path, MAX_FILE_NAME, "%s%s", folder, file_name);

    if (strstr(file_name, ".bmp") != NULL) {
        display_draw_image(file_path);
    } else if (strstr(file_name, ".log") != NULL) {
        FILE *fp = fopen(file_path, "r");
        if (fp != NULL) {
            char text[MAX_TEXT_SIZE];
            if (fread(text, 1, MAX_TEXT_SIZE - 1, fp) > 0) {
                text[MAX_TEXT_SIZE - 1] = '\0';
                display_draw_string(0, 0, text, &Font8, BACKGROUND_COLOR, FONT_COLOR);
            }
            fclose(fp);
        } else {
            log_error("Could not open file %s", file_path);
        }
    }

    delay_ms(2000);
}

void redraw_bitmap(Bitmap *bmp);

void redraw_bitmap(Bitmap *bmp) {
    if (bmp != NULL && bmp->pxl_data != NULL) {
        display_draw_image_data(bmp->pxl_data, bmp->img_width, bmp->img_height);
    } else {
        fprintf(stderr, "Failed to load or process BMP file.\n");
    }
}

int centerX = DISPLAY_WIDTH / 2;
int centerY = DISPLAY_HEIGHT / 2;
Bitmap *load_bitmap(const char *filename);

Bitmap *load_bitmap(const char *filename) {
    Bitmap *bmp = malloc(sizeof(Bitmap));
    if (bmp == NULL) {
        return NULL;
    }

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        free(bmp);
        return NULL;
    }

    return bmp;
}

int main(void) {
    signal(SIGINT, intHandler);
    log_info("Starting...");

    display_init();
    buttons_init();

    char entries[MAX_ENTRIES][MAX_FILE_NAME];
    int num_entries = 0;
    int selected = 0;
    bool viewingImage = false;

    display_init();

    num_entries = get_entries(VIEWER_FOLDER, entries);
    if (num_entries < 0) {
        log_error("Error getting entries");
        return -1;
    }

    draw_menu(entries, num_entries, selected);
    while (true) {
        delay_ms(200);
        if (!viewingImage) {
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
                draw_file(VIEWER_FOLDER, entries[selected]);
                delay_ms(2000);
                display_clear(BACKGROUND_COLOR);
                draw_menu(entries, num_entries, selected);

            } else if (button_center() == 0) {
                while (button_center() == 0) {
                    delay_ms(1);
                }

                uint8_t *imageBuffer = malloc(sizeof(uint8_t) * IMG_SIZE);

                display_clear(WHITE);
                display_draw_string(centerX - 30, centerY, "Say Cheese!", &Font8, WHITE,
                                    FONT_COLOR);
                delay_ms(1000);
                viewingImage = true;
                camera_capture_data(imageBuffer, IMG_SIZE);

                char newImageFilename[MAX_FILE_NAME];
                snprintf(newImageFilename, MAX_FILE_NAME, "%sBM.bmp", VIEWER_FOLDER);

                camera_save_to_file(imageBuffer, IMG_SIZE, newImageFilename);
                Bitmap bmp;
                create_bmp(&bmp, imageBuffer);

                viewingImage = true;
                display_draw_image_data(bmp.pxl_data, 128, 128);

                while (viewingImage) {
                    if (button_right() == 0) {
                        while (button_right() == 0) {
                            delay_ms(1);
                        }
                        remove_color_channel(RED_CHANNEL, &bmp);
                        display_draw_image_data(bmp.pxl_data, 128, 128);
                        reset_pixel_data(&bmp);
                    }

                    if (button_left() == 0) {
                        while (button_left() == 0) {
                            delay_ms(1);
                        }
                        remove_color_channel(BLUE_CHANNEL, &bmp);
                        display_draw_image_data(bmp.pxl_data, 128, 128);
                        reset_pixel_data(&bmp);
                    }

                    if (button_up() == 0) {
                        while (button_up() == 0) {
                            delay_ms(1);
                        }
                        remove_color_channel(GREEN_CHANNEL, &bmp);
                        display_draw_image_data(bmp.pxl_data, 128, 128);
                        reset_pixel_data(&bmp);
                    }

                    if (button_down() == 0) {
                        while (button_down() == 0) {
                            delay_ms(1);
                        }
                        or_filter(&bmp);
                        display_draw_image_data(bmp.pxl_data, 128, 128);
                        reset_pixel_data(&bmp);
                    }
                    if (button_center() == 0) {
                        while (button_center() == 0) {
                            delay_ms(1);
                        }
                        Config config = {.port = "2240",
                                         .host = "ecen224.byu.edu",
                                         .payload = imageBuffer,
                                         .payload_size = IMG_SIZE,
                                         .hw_id = "E5A8EF0E5"};

                        int sockfd = client_connect(&config);
                        if (sockfd >= 0) {
                            client_send_image(sockfd, &config);
                            client_receive_response(sockfd);
                            client_close(sockfd);
                        } else {
                            printf("Failed to connect to the server.\n");
                        }

                        destroy_bmp(&bmp);
                        num_entries = get_entries(VIEWER_FOLDER, entries);
                        selected = num_entries - 1;
                        viewingImage = false;
                        display_clear(BACKGROUND_COLOR);
                        num_entries = get_entries(VIEWER_FOLDER, entries);
                        draw_menu(entries, num_entries, selected);
                    }
                }
            }
        }
    }
    return 0;
}

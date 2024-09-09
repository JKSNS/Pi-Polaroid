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

#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

char entries[MAX_ENTRIES][MAX_FILE_NAME];
int num_entries = 0, selected = 0;
bool imageDisplayed = false;
Bitmap bmp;

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

void capture_and_display_image();
void update_menu();
void apply_filter_and_display(Color color);
void or_filter_and_display();
void cleanup_and_return_to_menu();

void capture_and_display_image() {
    uint8_t buf[IMG_SIZE];
    camera_capture_data(buf, IMG_SIZE);
    camera_save_to_file(buf, IMG_SIZE, VIEWER_FOLDER "doorbell.bmp");
    create_bmp(&bmp, buf);
    imageDisplayed = true;
}

void update_menu() {
    num_entries = get_entries(VIEWER_FOLDER, entries);
    draw_menu(entries, num_entries, selected);
}

void apply_filter_and_display(Color color) {
    remove_color_channel(color, &bmp);
}

void or_filter_and_display() {
    or_filter(&bmp);
}

void cleanup_and_return_to_menu() {
    destroy_bmp(&bmp);
    imageDisplayed = false;
    update_menu();
}

int main(void) {
    signal(SIGINT, intHandler);
    display_init();
    buttons_init();
    num_entries = get_entries(VIEWER_FOLDER, entries);
    draw_menu(entries, num_entries, selected);

    while (true) {
        delay_ms(200);

        if (!imageDisplayed) {
            if (button_right() == 0) {
                while (button_right() == 0) {
                    delay_ms(1);
                }
                draw_file(VIEWER_FOLDER, entries[selected]);
                imageDisplayed = true;
            }

            if (button_center() == 0) {
                while (button_center() == 0) {
                    delay_ms(1);
                }
                capture_and_display_image();
            }
        } else {
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

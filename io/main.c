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

#define VIEWER_FOLDER "viewer/"
#define MAX_ENTRIES 8
#define MAX_FILE_NAME 100
#define MAX_TEXT_SIZE 400

#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

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
            display_draw_string(0, i * 16, entries[i], &Font8, SELECTED_BG_COLOR, SELECTED_FONT_COLOR);
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

int main(void) {
    signal(SIGINT, intHandler);
    log_info("Starting...");

    display_init();
    buttons_init();

    char entries[MAX_ENTRIES][MAX_FILE_NAME];
    int num_entries = 0;
    int selected = 0;

    num_entries = get_entries(VIEWER_FOLDER, entries);
    if (num_entries < 0) {
        log_error("Error getting entries");
        return -1;
    }

    draw_menu(entries, num_entries, selected);

    while (true) {
        delay_ms(200);

        if (button_up() == 0) {
            while (button_up() == 0) {
                delay_ms(1);
            }
            selected = (selected - 1 + num_entries) % num_entries;
            printf("%d", selected);
            draw_menu(entries, num_entries, selected);
        }

        if (button_down() == 0) {
            while (button_down() == 0) {
                delay_ms(1);
            }
            selected = (selected + 1) % num_entries;
            printf("%d", selected);
            draw_menu(entries, num_entries, selected);
        }

        if (button_center() == 0) {
            while (button_center() == 0) {
                delay_ms(1);
            }
            display_clear(BACKGROUND_COLOR);
            draw_file(VIEWER_FOLDER, entries[selected]);
            draw_menu(entries, num_entries, selected);
        }
    }

    return 0;
}

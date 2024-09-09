#include "lib/buttons.h"
#include "lib/colors.h"
#include "lib/device.h"
#include "lib/display.h"
#include "lib/fonts/fonts.h"
#include "lib/log.h"
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

#define NUMBER_OF_COLORS 5
uint16_t COLORS[NUMBER_OF_COLORS] = {RED, GREEN, BLUE, YELLOW, CYAN};

void intHandler(int dummy) {
    display_exit();
    exit(0);
}

int main(void) {
    signal(SIGINT, intHandler);

    display_init();
    buttons_init();
    log_set_level(LOG_INFO);

    while (true) {
        delay_ms(200);

        if (button_up() == 0) {
            display_clear(WHITE);

            display_draw_rectangle(0, 0, DISPLAY_WIDTH, 20, GREEN, true, 1);

            display_draw_rectangle(0, DISPLAY_HEIGHT - 20, DISPLAY_WIDTH, DISPLAY_HEIGHT, RED, true,
                                   1);

            int centerX = DISPLAY_WIDTH / 2;
            int centerY = DISPLAY_HEIGHT / 2;
            int circleRadius = 30;
            int poundWidth = 50;
            int poundHeight = 50;

            int poundPosX = centerX - poundWidth / 2;
            int poundPosY = centerY - poundHeight / 2;

            poundPosX -= circleRadius - poundWidth / 2;
            poundPosY -= circleRadius - poundHeight / 2;

            display_draw_line(poundPosX + 5, centerY - 10, poundPosX + poundWidth + 5, centerY - 10,
                              BLACK, 3);
            display_draw_line(poundPosX + 5, centerY + 10, poundPosX + poundWidth + 5, centerY + 10,
                              BLACK, 3);
            display_draw_line(centerX - 10, poundPosY + 5, centerX - 10,
                              poundPosY + poundHeight + 5, BLACK,
                              3);
            display_draw_line(centerX + 10, poundPosY + 5, centerX + 10,
                              poundPosY + 5 + poundHeight, BLACK,
                              3);

            display_draw_circle(centerX, centerY, circleRadius, YELLOW, false, 3);

        } else if (button_down() == 0) {
            display_clear(WHITE);
        } else if (button_right() == 0) {
            display_clear(WHITE);
            for (int i = 0; i < 5; i++) {
                display_draw_string(10, 10 + i * 15, "Hello world!", &Font8, WHITE,
                                    COLORS[i % NUMBER_OF_COLORS]);
            }
        } else if (button_left() == 0) {
            display_clear(WHITE);
            const sFONT *fonts[5] = {&Font8, &Font12, &Font16, &Font20, &Font24};
            int posY = 10;

            for (int i = 0; i < 10; i++) {
                char buffer[2] = {'0' + i, '\0'};
                display_draw_string(10 + i * (DISPLAY_WIDTH / 10), posY, buffer,
                                    (sFONT *)fonts[i % 5], COLORS[i % NUMBER_OF_COLORS], WHITE);
            }

            posY += 25;

            for (int i = 0; i < 10; i++) {
                char buffer[2] = {'A' + i, '\0'};
                display_draw_string(10 + i * (DISPLAY_WIDTH / 10), posY, buffer,
                                    (sFONT *)fonts[i % 5], COLORS[i % NUMBER_OF_COLORS], WHITE);
            }

        } else if (button_center() == 0) {
            display_clear(WHITE);
            display_draw_image("pic/byu_og.bmp");
        }
    }
    return 0;
}

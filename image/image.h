
/*
Lab 4 - Image
*/
#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LOAD_ERROR 1
#define LOAD_SUCCESS 0
#define BMP_FILE_HEADER_SIZE 14
#define SAVE_ERROR 1
#define SAVE_SUCCESS 0

// Pixels in a bitmap file are arranged in the following format. Each pixel is
// three bytes of data, ordered by blue, green, red, with each color as one
// byte.
//
//               pixel x                   pixel x+1
//                   |                          |
//     +-------------+------------+-------------+------------+
//     v                          v                          v
//     +--------+--------+--------+--------+--------+--------+
// ... |   B    |   G    |   R    |   B    |   G    |   R    | ...
//     +--------+--------+--------+--------+--------+--------+
//       1 byte   1 byte   1 byte   1 byte   1 byte   1 byte
//
// To access the blue color in the xth pixel, you would index into pixel data by
// x * 3. To access the green color in the xth pixel, you would index into the
// pixel data by x * 3 + 1. To access the red color in the xth pixel, you would
// index into the pixel data by x * 3 + 2.

typedef enum Color { BLUE_CHANNEL, GREEN_CHANNEL, RED_CHANNEL } Color;

typedef struct {
    FILE *img;
    uint8_t file_header[BMP_FILE_HEADER_SIZE]; // 14 bytes
    uint8_t *dib_header;                       // Variable
    uint8_t *pxl_data;                         // Pixel data for image
    uint8_t *pxl_data_cpy;                     // Copy of pixel data
    uint8_t pxl_data_offset;                   // Location of pxl data in img
    uint32_t img_width;                        // Image width
    uint32_t img_height;                       // Image height
    uint32_t file_size;                        // Size of the file
    uint32_t pxl_data_size;                    // Size of pixel data array
} Bitmap;

#include <stdlib.h> // For malloc and free

uint8_t load_img(char *filepath, Bitmap *bmp);
void reset_pixel_data(Bitmap *bmp);
uint8_t save_img(char *imgname, Bitmap *bmp);
void remove_color_channel(Color color, Bitmap *bmp);
void grayscale(Bitmap *bmp);
void or_filter(Bitmap *bmp);

#endif // IMAGE_H
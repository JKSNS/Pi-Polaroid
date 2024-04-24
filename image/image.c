/*
This is the driver file for the image library. All function definitions live in here.
*/

#include "image.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------------------------------------
// Image loading/saving functions
// --------------------------------------------------------------------------

uint8_t load_img(char *filepath, Bitmap *bmp) {
    // Opens file in binary mode to correctly handle binary data
    bmp->img = fopen(filepath, "rb");
    if (!bmp->img) {
        return LOAD_ERROR;
    }

    // Read in the bitmap file header
    fread(bmp->file_header, sizeof(uint8_t), BMP_FILE_HEADER_SIZE, bmp->img);

    // Correctly calculate filesize using bitwise operations
    bmp->file_size = (bmp->file_header[5] << 24) | (bmp->file_header[4] << 16) |
                     (bmp->file_header[3] << 8) | bmp->file_header[2];

    // Correctly calculate start location of pixel data
    bmp->pxl_data_offset = (bmp->file_header[13] << 24) | (bmp->file_header[12] << 16) |
                           (bmp->file_header[11] << 8) | bmp->file_header[10];

    // Calculate DIB header size and allocate memory
    uint32_t dib_header_size = bmp->pxl_data_offset - BMP_FILE_HEADER_SIZE;
    bmp->dib_header = (uint8_t *)malloc(dib_header_size);
    fread(bmp->dib_header, sizeof(uint8_t), dib_header_size, bmp->img);

    // Get image width and height from the DIB header
    bmp->img_width = (bmp->dib_header[7] << 24) | (bmp->dib_header[6] << 16) |
                     (bmp->dib_header[5] << 8) | bmp->dib_header[4];
    bmp->img_height = (bmp->dib_header[11] << 24) | (bmp->dib_header[10] << 16) |
                      (bmp->dib_header[9] << 8) | bmp->dib_header[8];

    // Calculate pixel data size considering BMP row padding
    int rowSize =
        (bmp->img_width * 3 + 3) & ~3; // Rows are padded to the nearest multiple of 4 bytes
    bmp->pxl_data_size = rowSize * bmp->img_height;
    bmp->pxl_data = (uint8_t *)malloc(bmp->pxl_data_size);
    fseek(bmp->img, bmp->pxl_data_offset, SEEK_SET); // Move to the start of pixel data
    fread(bmp->pxl_data, sizeof(uint8_t), bmp->pxl_data_size, bmp->img);

    // Create a copy of image data for reset
    bmp->pxl_data_cpy = (uint8_t *)malloc(bmp->pxl_data_size);
    memcpy(bmp->pxl_data_cpy, bmp->pxl_data, bmp->pxl_data_size);

    // Close the file
    fclose(bmp->img);
    bmp->img = NULL; // Ensure the file pointer is not used accidentally after closing.

    return LOAD_SUCCESS;
}

void reset_pixel_data(Bitmap *bmp) { memcpy(bmp->pxl_data, bmp->pxl_data_cpy, bmp->pxl_data_size); }

uint8_t save_img(char *imgname, Bitmap *bmp) {
    // Open file in binary mode for writing
    FILE *save_img = fopen(imgname, "wb");
    if (!save_img) {
        fprintf(stderr, "Could not save the file with that name. Change the name and try again.\n");
        return SAVE_ERROR;
    }

    // Write BMP header
    fwrite(bmp->file_header, sizeof(uint8_t), BMP_FILE_HEADER_SIZE, save_img);

    // Write DIB header
    fwrite(bmp->dib_header, sizeof(uint8_t), bmp->pxl_data_offset - BMP_FILE_HEADER_SIZE, save_img);

    // Write pixel data, considering BMP padding if necessary
    fwrite(bmp->pxl_data, sizeof(uint8_t), bmp->pxl_data_size, save_img);

    // Close the file
    fclose(save_img);
    return SAVE_SUCCESS;
}

// --------------------------------------------------------------------------
// Image manipulation functions
// --------------------------------------------------------------------------

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

// Convert the image to grayscale
void grayscale(Bitmap *bmp) {
    for (uint32_t i = 0; i < bmp->pxl_data_size; i += 3) {
        // Calculate the grayscale value using the luminance method
        uint8_t red = bmp->pxl_data[i + 2];
        uint8_t green = bmp->pxl_data[i + 1];
        uint8_t blue = bmp->pxl_data[i];
        uint8_t luminance = (uint8_t)(0.299 * red + 0.587 * green + 0.114 * blue);

        // Set the RGB components to the calculated luminance value
        bmp->pxl_data[i] = bmp->pxl_data[i + 1] = bmp->pxl_data[i + 2] = luminance;
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
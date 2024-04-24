#include <stdlib.h>
#include <string.h>

#include "image.h"

uint8_t create_bmp(Bitmap *dst, uint8_t *src) {
    int offset = 0;
    dst->img = NULL;

    // Copy in the bitmap file header
    memcpy(dst->file_header, src, BMP_FILE_HEADER_SIZE);
    offset += BMP_FILE_HEADER_SIZE;

    // Get filesize
    dst->file_size = (dst->file_header[5] << 8 * 3) | (dst->file_header[4] << 8 * 2) |
                     (dst->file_header[3] << 8 * 1) | (dst->file_header[2]);

    // Get start location of pixel data
    dst->pxl_data_offset = (dst->file_header[13] << 8 * 3) | (dst->file_header[12] << 8 * 2) |
                           (dst->file_header[11] << 8 * 1) | (dst->file_header[10]);

    // // Get DIB header data
    uint8_t dib_header_size = dst->pxl_data_offset - BMP_FILE_HEADER_SIZE;
    dst->dib_header = (uint8_t *)malloc((dib_header_size) * sizeof(uint8_t));
    memcpy(dst->dib_header, src + offset, dib_header_size);
    offset += dib_header_size;

    // Get image width
    dst->img_width = (dst->dib_header[7] << 8 * 3) | (dst->dib_header[6] << 8 * 2) |
                     (dst->dib_header[5] << 8 * 1) | (dst->dib_header[4]);

    // Get image height
    dst->img_height = (dst->dib_header[11] << 8 * 3) | (dst->dib_header[10] << 8 * 2) |
                      (dst->dib_header[9] << 8 * 1) | (dst->dib_header[8]);

    if (dst->img_height < 0) {
        dst->img_height *= -1;
    }

    // Get image data
    dst->pxl_data_size = dst->img_width * dst->img_height * 3;
    dst->pxl_data = malloc(sizeof(uint8_t) * dst->pxl_data_size);
    memcpy(dst->pxl_data, src + offset, dst->pxl_data_size);

    // Create copy of image data for reset
    dst->pxl_data_cpy = malloc(sizeof(uint8_t) * dst->pxl_data_size);
    memcpy(dst->pxl_data_cpy, dst->pxl_data, dst->pxl_data_size);

    return LOAD_SUCCESS;
}

void destroy_bmp(Bitmap *bmp) {
    free(bmp->dib_header);
    free(bmp->pxl_data);
    free(bmp->pxl_data_cpy);
}

void reset_pixel_data(Bitmap *bmp) {
    bmp->pxl_data = memcpy(bmp->pxl_data, bmp->pxl_data_cpy, bmp->pxl_data_size);
}

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

uint8_t *get_pxl_data(Bitmap *bmp) { return bmp->pxl_data; }

uint8_t *get_original_pxl_data(Bitmap *bmp) { return bmp->pxl_data_cpy; }

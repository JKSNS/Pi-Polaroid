#include "image.h"
#include <stdio.h>

int main() {
    // Allocate memory for struct
    Bitmap original_image;

    // Load in data from image into struct
    if (load_img("./original.bmp", &original_image) == LOAD_ERROR) {
        printf("Unable to load image. Exiting...\n");
        return 1;
    }

    // Process the image to remove the green color channel
    remove_color_channel(GREEN_CHANNEL, &original_image);
    if (save_img("green_mask.bmp", &original_image) == SAVE_ERROR) {
        printf("Unable to save image with green channel removed. Exiting...\n");
        return 1;
    }
    reset_pixel_data(&original_image);

    // Remove the blue color channel and save the image
    remove_color_channel(BLUE_CHANNEL, &original_image);
    if (save_img("blue_mask.bmp", &original_image) == SAVE_ERROR) {
        printf("Unable to save image with blue channel removed. Exiting...\n");
        return 1;
    }
    reset_pixel_data(&original_image);

    // Remove the red color channel and save the image
    remove_color_channel(RED_CHANNEL, &original_image);
    if (save_img("red_mask.bmp", &original_image) == SAVE_ERROR) {
        printf("Unable to save image with red channel removed. Exiting...\n");
        return 1;
    }
    reset_pixel_data(&original_image);

    // Apply grayscale filter and save the image
    grayscale(&original_image);
    if (save_img("grayscale.bmp", &original_image) == SAVE_ERROR) {
        printf("Unable to save grayscale image. Exiting...\n");
        return 1;
    }
    reset_pixel_data(&original_image);

    // Apply "or filter" and save the image
    or_filter(&original_image);
    if (save_img("or_filter.bmp", &original_image) == SAVE_ERROR) {
        printf("Unable to save image with 'or filter' applied. Exiting...\n");
        return 1;
    }
    reset_pixel_data(&original_image);

    // Free allocated resources, if any (not shown here, but consider if your implementation
    // requires it)

    return 0;
}

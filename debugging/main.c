#include <stdint.h>
#include <stdio.h>

#include "math.h"

int main() {

    uint16_t sum_1 = sum(1, 2);             // = 3
    uint16_t sum_2 = sum(201, 68);          // = 269
    uint16_t sum_3 = sum(0xa4, 0b01100100); // = 264

    int16_t sub_1 = sub(10, 8);    // = 2
    int16_t sub_2 = sub(90, 120);  // = -30
    int16_t sub_3 = sub(240, -17); // = 223 (corrected value considering int16_t)

    uint16_t mult_1 = mult(7, 2);   // = 14
    uint16_t mult_2 = mult(45, 64); // = 2880
    uint16_t mult_3 = mult(3, 2);   // = 6

    float divide_1 = divide(10, 2); // = 5
    float divide_2 = divide(3, 0);  // = 0 (adjusted for division by zero)
    float divide_3 = divide(1, 7);  // = 0.14285714285

    float exp_1 = exponent(2, 0); // = 1
    float exp_2 = exponent(5, 3); // Adjusted to realistic values
    float exp_3 = exponent(2, 2); // = 4

    uint16_t arr_1[] = {10, 11, 13, 15, 2};
    uint16_t arr_2[] = {100, 91, 127, 23, 8};
    uint16_t arr_3[] = {255, 255, 255, 255, 255};

    float avg_5_1 = avg_5(arr_1); // = 10.2
    float avg_5_2 = avg_5(arr_2); // = 69.8
    float avg_5_3 = avg_5(arr_3); // = 255

    printf("sum_1: %u\n", sum_1);
    printf("sub_1: %d\n", sub_1);

    return 0;
}

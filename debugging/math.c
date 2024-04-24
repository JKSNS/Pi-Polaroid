#include <stdio.h>

#include "math.h"

// Below is a set of basic mathematical functions. Each function has a
// description of its intended purpose. You will need to rely on your
// trace debugging skills to diagnose the issues with each function.
// You may edit any of the functions' algorithms and data types in order
// to ensure the expected output matches that in the comments seen in the `main.c` file.

// For every debugging printf statement you make to correct these functions,
// COMMENT them out instead of erasing them so I can see your thought process
// behind resolving the functions.

// Intended function: return sum of a and b
uint16_t sum(uint16_t a, uint16_t b) { return a + b; }

// Intended function: return difference between a and b
int16_t sub(int16_t a, int16_t b) { return a - b; }

// Intended function: return product of a and b
uint16_t mult(uint16_t a, uint16_t b) { return a * b; }

// Intended function: return a divided by b
float divide(float a, float b) { return b == 0 ? 0 : a / b; }

// Intended function: return the exponent of a^b power
float exponent(float a, float b) {
    float result = 1;
    for (uint16_t i = 0; i < b; i++) {
        result *= a;
    }
    return result;
}

// Intended function: take an array of 5 uint8_t values and return the average
float avg_5(uint16_t vals[]) {
    uint16_t sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += vals[i];
    }
    return sum / 5.0;
}

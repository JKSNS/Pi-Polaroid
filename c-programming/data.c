#include <stdint.h>
#include <stdio.h>

void pCE(uint8_t num) { printf("char equivalent of %u is '%c'\n", num, num); }

int main() {

    printf("Hex equivalent of 3735928559: 0x%X\n", 3735928559);

    uint8_t num1 = 55;
    uint8_t num2 = 56;
    uint8_t num3 = 57;

    pCE(num1);
    pCE(num1);
    pCE(num1);

    printf("Three chars: %c, %c, %c\n", num1, num2, num3);

    printf("Integer: %d\n", 100);
    printf("Floating point: %f\n", 123.456);
    printf("Single character: %c\n", 'Z');
    printf("String: %s\n", "Hello, World!");
    printf("Hexadecimal: 0x%x\n", 255);

    printf("Char as integer: %d\n", 'A');

    return 0;
}

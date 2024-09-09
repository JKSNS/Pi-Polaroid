#include <stdio.h>

char gcfr() { return 'A'; }

float gffr() { return 3.14f; }

int main() {
    char myCharacter = gcfr();
    printf("Fixed character: %c\n", myCharacter);

    float myFloat = gffr();
    printf("Fixed float: %.2f\n", myFloat);

    printf("Hello, World!\n");
    return 0;
}

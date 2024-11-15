// operations.c
#include "operations.h"
#include <stdio.h>

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        printf("MATH ERROR: Division by zero.\n");
        return 0;
    }
    return a / b;
}

int modulo(int a, int b) {
    if (b == 0) {
        printf("MATH ERROR: Modulo by zero.\n");
        return 0;
    }
    return a % b;
}

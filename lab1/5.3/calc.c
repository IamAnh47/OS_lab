// calc.c
#include <stdio.h>
#include <stdlib.h>
#include "operations.h"

int main() {
    double a, b;
    char op;
    double result;

    printf("Enter calculation: ");
    while (scanf("%lf %c %lf", &a, &op, &b) == 3) {
        switch (op) {
            case '+':
                result = add(a, b);
                break;
            case '-':
                result = subtract(a, b);
                break;
            case 'x':
                result = multiply(a, b);
                break;
            case '/':
                result = divide(a, b);
                break;
            case '%':
                result = modulo((int)a, (int)b);  
                break;
            default:
                printf("SYNTAX ERROR\n");
                printf("Enter calculation: ");
                continue;
        }

        if (result == (int)result) {
            printf("Result: %.0lf\n", result);
        } else {
            printf("Result: %.2lf\n", result);
        }
        printf("Enter calculation: ");
    }
    return 0;
}

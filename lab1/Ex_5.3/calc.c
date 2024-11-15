// calc.c
#include <stdio.h>
#include <stdlib.h>
#include "operations.h"

int main() {
    double a, b;
    char op;
    double result;

    printf("Enter calculation (e.g., 5 + 3): ");
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
                continue;
        }
        printf("Result: %.2lf\n", result);
        printf("Enter calculation (e.g., 5 + 3) or Ctrl+C to exit: ");
    }
    return 0;
}

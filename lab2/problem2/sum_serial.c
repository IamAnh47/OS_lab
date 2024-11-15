#include <stdio.h>
#include <stdlib.h>

// Tinh tong tu 1 den n
unsigned long long sum_serial(unsigned long long n) {
    // unsigned long long sum = 0;
    // for (unsigned long long i = 1; i <= n; i++) {
    //     sum += i;
    // }
    // return sum;
    return (n * (n + 1)) / 2; 
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    unsigned long long n = atoll(argv[1]);
    unsigned long long result = sum_serial(n);

    printf("Sum of numbers from 1 to %llu is: %llu\n", n, result);
    return 0;
}

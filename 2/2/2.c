#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdarg.h>

double geometric_mean(int n, ...) {
    va_list vl;
    double product = 1.0;

    va_start(vl, n);
    for (int i = 0; i < n; i++) {
        double num = va_arg(vl, double);
        product *= num;
    }
    va_end(vl);

    return pow(product, 1.0 / n);
}

double recursive_pow(double base, int exponent) {
    if (exponent == 0) {
        return 1.0;
    } else if (exponent < 0) {
        return 1.0 / recursive_pow(base, -exponent);
    } else if (exponent % 2 == 0) {
        double half = recursive_pow(base, exponent / 2);
        return half * half;
    } else {
        return base * recursive_pow(base, exponent - 1);
    }
}

int main() {
    double mean = geometric_mean(3, 2.5, 4.5, 8.0);
    printf("Result of the geometric mean: %g\n", mean);

    double result = recursive_pow(4.5, 2);
    printf("The result of recursive pow: %g\n", result);

    return 0;
}

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double integral(double (*func)(double), double a, double b, double eps);
double func_a(double x);
double func_b(double x);
double func_c(double x);
double func_d(double x);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <epsilon>\n", argv[0]);
        return 1;
    }

    double epsilon = atof(argv[1]);
    if (epsilon > 1) {
        printf("incorrect epsilon\n");
        return 1;
    }

    double result_a = integral(func_a, epsilon, 1, epsilon);
    double result_b = integral(func_b, 0, 1, epsilon);
    double result_c = integral(func_c, 0, 1 - epsilon, epsilon);
    double result_d = integral(func_d, epsilon, 1, epsilon);

    printf("---------------------------------------------------------\n");
    printf("Integral Function       | Result\n");
    printf("------------------------|------------------------\n");
    printf("Integral a (ln(1+x)/x)  | %.10f\n", result_a);
    printf("Integral b (exp(-x^2/2))| %.10f\n", result_b);
    printf("Integral c (ln(1/(1-x)))| %.10f\n", result_c);
    printf("Integral d (x^x)        | %.10f\n", result_d);
    printf("------------------------|------------------------\n");

    return 0;
}

double integral(double (*func)(double), double a, double b, double eps) {
    double n = 1.0;
    double h, sum = 0.0, prev_sum = 0.0;
    do {
        prev_sum = sum;
        sum = 0.0;
        h = (b - a) / n;
        for (int i = 0; i < n; i++) {
            sum += 0.5 * h * (func(a + i * h) + func(a + (i + 1) * h));
        }
        n *= 2;
    } while (fabs(sum - prev_sum) > eps);

    return sum;
}

// a. ln(1+x)/x
double func_a(double x) {
    if (x == 0) return 0;
    return log(1 + x) / x;
}

// b. e^(-x^2/2)
double func_b(double x) {
    return exp(-x * x / 2);
}

// c. ln(1/(1-x))
double func_c(double x) {
    if (x == 1) return 0;
    return log(1 / (1 - x));
}

// d. x^x
double func_d(double x) {
    if (x == 0) return 0;
    return pow(x, x);
}

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

double my_atof(const char *str) {
    double result = 0.0;
    double fraction = 0.0;
    int sign = 1;
    int decimal_position = 0;

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        result = result * 10.0 + (*str - '0');
        str++;
    }

    if (*str == '.') {
        str++;
        while (*str >= '0' && *str <= '9') {
            fraction = fraction * 10.0 + (*str - '0');
            str++;
            decimal_position++;
        }
    }

    result += fraction / pow(10.0, decimal_position);
    return sign * result;
}

void validate_epsilon(const char *input) {
    int decimal_count = 0;

    for (const char *ptr = input; *ptr; ptr++) {
        if (*ptr == '.') {
            decimal_count++;
            if (decimal_count > 1) {
                printf("Invalid input for epsilon: multiple decimal points.\n");
                return;
            }
        } else if (!isdigit(*ptr) && *ptr != '+' && *ptr != '-') {
            printf("Invalid input for epsilon: contains non-numeric characters.\n");
            return;
        }
    }

    if (decimal_count == 0) {
        printf("Invalid input for epsilon: no decimal point found.\n");
        return;
    }
}

// a
double sum_a(double x, double epsilon) {
    double sum = 1.0;
    double term = 1.0;
    int n = 1;

    while (fabs(term) > epsilon) {
        term *= x / n;
        sum += term;
        n++;
    }

    return sum;
}

// b
double sum_b(double x, double epsilon) {
    double sum = 1.0;
    double term = 1.0;
    int n = 1;

    while (fabs(term) > epsilon) {
        term *= -1.0 * x * x / (2 * n * (2 * n - 1));
        sum += term;
        n++;
    }

    return sum;
}

// c
double sum_c(double x, double epsilon) {
    if (x <= -1.0 || x >= 1.0) {
        printf("Sum c is not convergent for x = %.10lf. Valid range is (-1, 1).\n", x);
        return NAN;
    }
    
    double sum = 1.0;
    double term = 1.0;
    int n = 0;

    while (fabs(term) > epsilon) {
        n++;
        term *= (27 * n * n * n * x * x) / ((3 * n - 1) * (3 * n - 2) * 3 * n);
        sum += term;
    }
    return sum;
}

// d
double double_factorial(int n) {
    if (n <= 1) return 1.0;
    double result = 1.0;
    for (int i = n; i > 1; i -= 2) {
        result *= i;
    }
    return result;
}

double sum_d(double x, double epsilon) {
    if (x <= -1.0 || x >= 1.0) {
        printf("Sum d is not convergent for x = %.10lf. Valid range is [-1, 1].\n", x);
        return NAN;
    }
    
    double sum = 0.0;
    double term;
    int n = 1;
    double sign = -1.0;
    const int MAX_ITERATIONS = 1000;
    int iteration_count = 0;

    do {
        double double_fact_2n_minus_1 = double_factorial(2 * n - 1);
        double double_fact_2n = double_factorial(2 * n);
        term = sign * double_fact_2n_minus_1 * pow(x, 2 * n) / double_fact_2n;

        if (isnan(term)) {
            break;
        }

        sum += term;
        sign = -sign;
        n++;
        iteration_count++;

        if (iteration_count > MAX_ITERATIONS) {
            break;
        }

    } while (fabs(term) > epsilon);
    return sum;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <epsilon> <x>\n", argv[0]);
        return 1;
    }

    validate_epsilon(argv[1]);
    double epsilon = my_atof(argv[1]);
    
    if (epsilon <= 0) {
        printf("Invalid input for epsilon: must be greater than zero.\n");
        return 1;
    }

    double x = my_atof(argv[2]);

    printf("\nUsing epsilon: %.10lf and x: %.10lf\n", epsilon, x);
    printf("---------------------------------------------------------\n");
    printf("%-20s | %-10s\n", "Series", "Result");
    printf("---------------------|-----------------\n");

    printf("Sum a                | %.10lf\n", sum_a(x, epsilon));
    printf("Sum b                | %.10lf\n", sum_b(x, epsilon));
    printf("Sum c                | %.10lf\n", sum_c(x, epsilon));
    printf("Sum d                | %.10lf\n", sum_d(x, epsilon));
    printf("---------------------|-----------------\n");

    return 0;
}

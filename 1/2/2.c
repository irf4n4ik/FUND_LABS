#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITERATIONS 1000000

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

int validate_epsilon(const char *input) {
    int decimal_count = 0;

    for (const char *ptr = input; *ptr; ptr++) {
        if (*ptr == '.') {
            decimal_count++;
            if (decimal_count > 1) {
                printf("Invalid input for epsilon: multiple decimal points.\n");
                return 1;
            }
        } else if (!isdigit(*ptr) && *ptr != '+' && *ptr != '-') {
            printf("Invalid input for epsilon: contains non-numeric characters.\n");
            return 1;
        }
    }

    if (decimal_count == 0) {
        printf("Invalid input for epsilon: no decimal point found.\n");
        return 1;
    }

    return 0;
}

//e
double calculate_e_limit(double epsilon) {
    double e_prev = 0.0, e_current = 1.0;
    int n = 1, iteration = 0;

    while (fabs(e_current - e_prev) > epsilon && iteration < MAX_ITERATIONS) {
        e_prev = e_current;
        e_current = pow((1.0 + 1.0 / n), n);
        n++;
        iteration++;
    }

    return e_current;
}

double calculate_e_series(double epsilon) {
    double e = 1.0, term = 1.0;
    int n = 1, iteration = 0;

    while (term > epsilon && iteration < MAX_ITERATIONS) {
        term /= n;
        e += term;
        n++;
        iteration++;
    }

    return e;
}

double calculate_e_equation(double epsilon) {
    double x = 2.0, step = 1.0;
    int iteration = 0;

    while (fabs(log(x) - 1.0) > epsilon && iteration < MAX_ITERATIONS) {
        x += step * (1.0 - log(x));
        iteration++;
    }

    return x;
}

//Pi
double calculate_pi_limit(double epsilon) {
    double pi = 0.0;
    double term;
    int n = 0, iteration = 0;

    do {
        term = pow(-1, n) / (2.0 * n + 1.0);
        pi += term;
        n++;
        iteration++;
    } while (fabs(term) > epsilon && iteration < MAX_ITERATIONS);

    return 4.0 * pi;
}

double calculate_pi_series(double epsilon) {
    double pi = 0.0;
    double term;
    int n = 1, iteration = 0;

    do {
        term = pow(-1, n - 1) / (2.0 * n - 1.0);
        pi += term;
        n++;
        iteration++;
    } while (fabs(term) > epsilon && iteration < MAX_ITERATIONS);

    return 4.0 * pi;
}

double calculate_pi_equation(double epsilon) {
    double x = 3.14;
    double step;
    int iteration = 0;

    do {
        double f_x = sin(x);
        double f_prime_x = cos(x);

        if (fabs(f_prime_x) < 1e-10) {
            break;
        }

        step = f_x / f_prime_x;
        x -= step;
        iteration++;
    } while (fabs(sin(x)) > epsilon && iteration < MAX_ITERATIONS);

    return x;
}

//ln2
double calculate_ln2_limit(double epsilon) {
    double ln2_prev = 0.0, ln2_current = 0.0;
    int n = 1, iteration = 0;

    do {
        ln2_prev = ln2_current;
        ln2_current = n * (pow(2, 1.0 / n) - 1);
        n++;
        iteration++;
    } while (fabs(ln2_current - ln2_prev) > epsilon && iteration < MAX_ITERATIONS);

    return ln2_current;
}

double calculate_ln2_series(double epsilon) {
    double ln2 = 0.0, term;
    int n = 1, iteration = 0;

    do {
        term = pow(-1, n - 1) / n;
        ln2 += term;
        n++;
        iteration++;
    } while (fabs(term) > epsilon && iteration < MAX_ITERATIONS);

    return ln2;
}

double calculate_ln2_equation(double epsilon) {
    double x = 1.0, step = 0.1;
    int iteration = 0;

    while (fabs(exp(x) - 2.0) > epsilon && iteration < MAX_ITERATIONS) {
        x += step * (2.0 - exp(x));
        iteration++;
    }

    return x;
}

//sqrt2
double calculate_sqrt2_limit(double epsilon) {
    double x = 1.0;
    int iteration = 0;

    while (fabs(x * x - 2.0) > epsilon && iteration < MAX_ITERATIONS) {
        x = (x + 2.0 / x) / 2.0;
        iteration++;
    }

    return x;
}

double calculate_sqrt2_series(double epsilon) {
    double x = 1.0, term;
    int n = 1, iteration = 0;

    do {
        term = (2.0 - x * x) / (2.0 * x);
        x += term;
        n++;
        iteration++;
    } while (fabs(term) > epsilon && iteration < MAX_ITERATIONS);

    return x;
}

double calculate_sqrt2_equation(double epsilon) {
    double x = 1.5, step = 0.1;
    int iteration = 0;

    while (fabs(x * x - 2.0) > epsilon && iteration < MAX_ITERATIONS) {
        x -= step * (x * x - 2.0);
        iteration++;
    }

    return x;
}

void print_result(const char *name, const char *method, double result) {
    printf("%-20s | %-15s | %.10f\n", name, method, result);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <epsilon>\n", argv[0]);
        return 1;
    }

    if (validate_epsilon(argv[1]) != 0) {
        return 1;
    }

    double epsilon = my_atof(argv[1]);
    
    if (epsilon <= 0) {
        printf("Invalid input for epsilon: must be greater than zero.\n");
        return 1;
    }

    printf("\nUsing epsilon: %.10Lf\n", epsilon);
    printf("---------------------------------------------------------\n");
    printf("%-20s | %-15s | %-10s\n", "Constant", "Method", "Result");
    printf("---------------------|-----------------|-----------------\n");

    print_result("e", "Limit", calculate_e_limit(epsilon));
    print_result("e", "Series", calculate_e_series(epsilon));
    print_result("e", "Equation", calculate_e_equation(epsilon));
    printf("---------------------|-----------------|-----------------\n");

    print_result("pi", "Limit", calculate_pi_limit(epsilon));
    print_result("pi", "Series", calculate_pi_series(epsilon));
    print_result("pi", "Equation", calculate_pi_equation(epsilon));
    printf("---------------------|-----------------|-----------------\n");

    print_result("ln(2)", "Limit", calculate_ln2_limit(epsilon));
    print_result("ln(2)", "Series", calculate_ln2_series(epsilon));
    print_result("ln(2)", "Equation", calculate_ln2_equation(epsilon));
    printf("---------------------|-----------------|-----------------\n");

    print_result("sqrt(2)", "Limit", calculate_sqrt2_limit(epsilon));
    print_result("sqrt(2)", "Series", calculate_sqrt2_series(epsilon));
    print_result("sqrt(2)", "Equation", calculate_sqrt2_equation(epsilon));
    printf("---------------------|-----------------|-----------------\n");

    return 0;
}

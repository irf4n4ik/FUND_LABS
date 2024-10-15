#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITERATIONS 1000000

typedef enum Errors {
    ERROR_NONE = 0,
    ERROR_INVALID_ARGUMENTS,
    ERROR_MULTIPLE_DECIMAL_POINTS,
    ERROR_NON_NUMERIC_CHARACTER,
    ERROR_NO_DECIMAL_POINT,
    ERROR_EPSILON_LESS_THAN_ZERO
} Errors;

Errors validate_epsilon(const char *input);
double my_atof(const char *str);
void print_error(Errors error);

Errors GetOpts(int argc, char** argv, double* epsilon) {
    if (argc > 2) {
        printf("Error: Too many arguments.\n");
        printf("Usage: %s <epsilon>\n", argv[0]);
        return ERROR_INVALID_ARGUMENTS;
    }
    if (argc < 2) {
        printf("Error: Missing epsilon argument.\n");
        printf("Usage: %s <epsilon>\n", argv[0]);
        return ERROR_INVALID_ARGUMENTS;
    }

    Errors err = validate_epsilon(argv[1]);
    if (err != ERROR_NONE) {
        print_error(err);
        return err;
    }

    *epsilon = my_atof(argv[1]);

    if (*epsilon <= 0) {
        print_error(ERROR_EPSILON_LESS_THAN_ZERO);
        return ERROR_EPSILON_LESS_THAN_ZERO;
    }

    return ERROR_NONE;
}

Errors validate_epsilon(const char *input) {
    int decimal_count = 0;

    for (const char *ptr = input; *ptr; ptr++) {
        if (*ptr == '.') {
            decimal_count++;
            if (decimal_count > 1) {
                return ERROR_MULTIPLE_DECIMAL_POINTS;
            }
        } else if (!isdigit(*ptr) && *ptr != '+' && *ptr != '-') {
            return ERROR_NON_NUMERIC_CHARACTER;
        }
    }

    if (decimal_count == 0) {
        return ERROR_NO_DECIMAL_POINT;
    }

    return ERROR_NONE;
}

void print_error(Errors error) {
    switch (error) {
        case ERROR_MULTIPLE_DECIMAL_POINTS:
            printf("Error: Multiple decimal points in epsilon.\n");
            break;
        case ERROR_NON_NUMERIC_CHARACTER:
            printf("Error: Non-numeric character in epsilon.\n");
            break;
        case ERROR_NO_DECIMAL_POINT:
            printf("Error: No decimal point found in epsilon.\n");
            break;
        case ERROR_EPSILON_LESS_THAN_ZERO:
            printf("Error: Epsilon must be greater than zero.\n");
            break;
        case ERROR_INVALID_ARGUMENTS:
            printf("Error: Invalid number of arguments.\n");
            printf("Usage: %s <epsilon>\n", "program_name");
            break;
        default:
            printf("Error: Unknown error.\n");
            break;
    }
}

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

    if (decimal_position > 0) {
        result += fraction / pow(10.0, decimal_position);
    }

    return sign * result;
}

double Factorial(int n){
    int answer = 1;
    for (int i = 2; i <= n; i++) {
        answer *= i;
    }

    return answer * 1.00;
}

double for_e_limit(double n) {
    return pow(1.0 + 1 / n, n);
}

// Вычисления:

// e
double e_limit(double epsilon) {
    double n = 1;
    double current_element = 1;
    double last_element = 0;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        n *= 2;
        current_element = for_e_limit(n);
    }
    return current_element;
}

double e_series(double epsilon) {
    double n = 1.0;
    double current_element = 1.0000;
    double summ = 1.0;
    while (fabs(current_element) >= epsilon) {
        current_element /= n;
        summ += current_element;
        n++;
    }
    return summ;
}

double e_equation(double epsilon) {
    double a = 1.00;
    double b = 5.00;
    double x;
    while (fabs(b - a) >= epsilon) {
        x = (b + a) / 2;
        if (log(x) > 1) {
            b = x;
        } else {
            a = x;
        }
    }
    return x;
}

// Pi
double pi_limit(double epsilon) {
    double n = 1.0;
    double term = 4.0;
    double pi = 0.0;
    double sign = 1.0;

    while (fabs(term) >= epsilon) {
        pi += term;
        n += 2;
        sign *= -1;
        term = sign * (4.0 / n);
    }

    return pi;
}

double pi_series(double epsilon) {
    double n = 1.0;
    double current_element = 4.0;
    double pi = 0.0;
    while (fabs(current_element) >= epsilon) {
        pi += current_element;
        n += 2;
        current_element = 4.0 * pow(-1, (int)(n / 2)) / n;
    }
    return pi;
}

double pi_equation(double epsilon) {
    double a = 3.0;
    double b = 4.0;
    double x;
    while (fabs(b - a) >= epsilon) {
        x = (b + a) / 2;
        if (sin(x) > 0) {
            a = x;
        } else {
            b = x;
        }
    }
    return x;
}

// ln(2)
double ln2_limit(double epsilon) {
    int n = 1;
    double current_element = (pow(2, 1.00 / n) - 1) * n;
    double last_element = 0;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        n++;
        current_element = (pow(2, 1.00 / n) - 1) * n;
    }
    return current_element;
}

double ln2_series(double epsilon) {
    int n = 2;
    double last_element = 0.0000;
    double current_element = 1.0000;
    while (fabs(current_element - last_element) >= epsilon) {
        last_element = current_element;
        current_element += (pow(-1, n - 1) / n) * 1.000;
        n++;
    }
    return current_element;
}

double ln2_equation(double epsilon) {
    double a = 0.00;
    double b = 3.00;
    double x;
    while (fabs(b - a) >= epsilon) {
        x = (b + a) / 2;
        if (exp(x) - 2 > 0) {
            b = x;
        } else {
            a = x;
        }
    }
    return x;
}

// sqrt(2)
double square_limit(double epsilon) {
    double current_element = 1.0;
    double last_element = 0;
    while (fabs(last_element - current_element) >= epsilon) {
        last_element = current_element;
        current_element = (last_element + 2 / last_element) / 2;
    }
    return current_element;
}

double square_series(double epsilon) {
    double current_element = 1.0;
    double next_element = (current_element + 2 / current_element) / 2;

    while (fabs(next_element - current_element) >= epsilon) {
        current_element = next_element;
        next_element = (current_element + 2 / current_element) / 2;
    }

    return next_element;
}

double square_equation(double epsilon) {
    double a = 0.00;
    double b = 3.00;
    double x;
    while (fabs(b - a) >= epsilon) {
        x = (b + a) / 2;
        if (pow(x, 2) > 2) {
            b = x;
        } else {
            a = x;
        }
    }
    return x;
}

void print_result(const char *name, const char *method, double result) {
    printf("%-20s | %-15s | %.10f\n", name, method, result);
}

int main(int argc, char *argv[]) {
    double epsilon;
    Errors err = GetOpts(argc, argv, &epsilon);
    if (err != ERROR_NONE) {
        return 1;
    }

    printf("\nUsing epsilon: %.10f\n", epsilon);
    printf("---------------------------------------------------------\n");
    printf("%-20s | %-15s | %-10s\n", "Constant", "Method", "Result");
    printf("---------------------|-----------------|-----------------\n");

    print_result("e", "Limit", e_limit(epsilon));
    print_result("e", "Series", e_series(epsilon));
    print_result("e", "Equation", e_equation(epsilon));
    printf("---------------------|-----------------|-----------------\n");

    print_result("Pi", "Limit", pi_limit(epsilon));
    print_result("Pi", "Series", pi_series(epsilon));
    print_result("Pi", "Equation", pi_equation(epsilon));
    printf("---------------------|-----------------|-----------------\n");

    print_result("ln(2)", "Limit", ln2_limit(epsilon));
    print_result("ln(2)", "Series", ln2_series(epsilon));
    print_result("ln(2)", "Equation", ln2_equation(epsilon));
    printf("---------------------|-----------------|-----------------\n");

    print_result("sqrt(2)", "Limit", square_limit(epsilon));
    print_result("sqrt(2)", "Series", square_series(epsilon));
    print_result("sqrt(2)", "Equation", square_equation(epsilon));
    printf("---------------------|-----------------|-----------------\n");

    return 0;
}

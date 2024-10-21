#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef enum {
    SUCCESS = 0,
    MEMORY_ALLOCATION_ERROR = -1,
    INVALID_PARAMETER_ERROR = -2
} StatusCode;

StatusCode taylor_expand(double epsilon, double a, double** taylor_coeffs, int max_degree, ...)
{
    if (taylor_coeffs == NULL || max_degree < 0 || epsilon <= 0.0 || !isfinite(a)) {
        return INVALID_PARAMETER_ERROR;
    }

    *taylor_coeffs = (double*)malloc((max_degree + 1) * sizeof(double));
    if (*taylor_coeffs == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    va_list args;
    va_start(args, max_degree);

    double* original_coeffs = (double*)malloc((max_degree + 1) * sizeof(double));
    if (original_coeffs == NULL) {
        free(*taylor_coeffs);
        return MEMORY_ALLOCATION_ERROR;
    }

    for (int i = 0; i <= max_degree; i++) {
        original_coeffs[i] = va_arg(args, double);
    }
    
    va_end(args);

    for (int i = 0; i <= max_degree; i++) {
        (*taylor_coeffs)[i] = original_coeffs[i];
        for (int j = 0; j < i; j++) {
            (*taylor_coeffs)[i] -= (*taylor_coeffs)[j] * pow(a, i - j);
        }
        if (fabs((*taylor_coeffs)[i]) < epsilon) {
            (*taylor_coeffs)[i] = 0.0;
        }
    }

    free(original_coeffs);
    return SUCCESS;
}

void print_coefficients(const double* coeffs, int degree, double a) {
    printf("Taylor series coefficients for polynomial expansion around (x - %lf):\n", a);
    for (int i = 0; i <= degree; i++) {
        printf("g_%d = %lf\n", i, coeffs[i]);
    }
}

int main() {
    double* taylor_coeffs = NULL;
    int max_degree = 2;
    double a = 1.0;
    double epsilon = 0.000001;

    StatusCode result = taylor_expand(epsilon, a, &taylor_coeffs, max_degree, 1.0, 3.0, 5.0);
    if (result != SUCCESS) {
        if (result == MEMORY_ALLOCATION_ERROR) {
            printf("Memory allocation error!\n");
        } else if (result == INVALID_PARAMETER_ERROR) {
            printf("Error: invalid input parameters!\n");
        }
        return result;
    }

    print_coefficients(taylor_coeffs, max_degree, a);

    free(taylor_coeffs);

    return 0;
}

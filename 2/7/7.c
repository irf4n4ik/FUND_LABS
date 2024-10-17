#include <stdio.h>
#include <math.h>
#include <stdlib.h>

enum StatusCode {
    SUCCESS,
    ERROR_INVALID_INPUT,
    ERROR_OVERFLOW,
    ERROR_MEMORY_ALLOCATION
};

double dichotomy(const double (*func)(double), double x0, double xk, double eps, enum StatusCode* status) {
    double xm;
    if (func(x0) * func(xk) > 0) {
        *status = ERROR_INVALID_INPUT;
        return NAN;
    }

    while (fabs(xk - x0) >= eps) {
        xm = (x0 + xk) / 2;
        if (func(x0) * func(xm) > 0) {
            x0 = xm;
        } else {
            xk = xm;
        }
    }

    *status = SUCCESS;
    return xm;
}

double func1(double x) {
    return 2.0 - x;
}

double func2(double x) {
    return x * x - 4.0;
}

enum StatusCode read_input(double* x0, double* xk, double* eps) {
    printf("Enter the interval boundaries [x0, xk]:\n");
    if (scanf("%lf %lf", x0, xk) != 2) {
        return ERROR_INVALID_INPUT;
    }

    printf("Enter precision eps: ");
    if (scanf("%lf", eps) != 1 || *eps <= 0) {
        return ERROR_INVALID_INPUT;
    }

    return SUCCESS;
}

int main(void) {
    double x0, xk, eps;
    enum StatusCode status = read_input(&x0, &xk, &eps);

    if (status != SUCCESS) {
        printf("Input error!\n");
        return status;
    }

    printf("Choose the equation:\n [1] for f(x) = 2 - x\n [2] for f(x) = x^2 - 4\n");
    int choice;
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice!\n");
        return ERROR_INVALID_INPUT;
    }

    double root;
    if (choice == 1) {
        root = dichotomy(func1, x0, xk, eps, &status);
    } else {
        root = dichotomy(func2, x0, xk, eps, &status);
    }

    if (status == SUCCESS) {
        printf("Root found: %lf\n", root);
    } else {
        printf("Error: No root found or invalid interval.\n");
    }

    return status;
}
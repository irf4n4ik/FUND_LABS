#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double f_a(double x) {
    return log(1 + x) / x;
}

double f_b(double x) {
    return exp(-x * x / 2);
}

double f_c(double x) {
    return log(1 / (1 - x));
}

double f_d(double x) {
    return pow(x, x);
}

double handle_singularity(double x, double (*f)(double)) {
    if (fabs(x) < 1e-10 && f == f_a) {
        return 1.0;
    } else if (fabs(x - 1) < 1e-10 && f == f_c) {
        return INFINITY;
    }
    return f(x);
}

double integrate(double (*f)(double), double a, double b, double epsilon) {
    int n = 1;
    double h = (b - a);
    double integral_old = (handle_singularity(a, f) + handle_singularity(b, f)) * h / 2.0;
    double integral_new;

    do {
        integral_new = 0.0;
        h /= 2.0;
        n *= 2;

        for (int i = 0; i < n; i++) {
            double x_mid = a + (i + 0.5) * h;
            double f_value = handle_singularity(x_mid, f);
            if (isinf(f_value) || isnan(f_value)) {
                continue;
            }
            integral_new += f_value;
        }

        integral_new = (integral_old + h * integral_new) / 2.0;
        if (fabs(integral_new - integral_old) < epsilon) {
            break;
        }
        integral_old = integral_new;
    } while (fabs(integral_new - integral_old) >= epsilon && h > epsilon);

    return integral_new;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <epsilon>\n", argv[0]);
        return 1;
    }

    double epsilon = atof(argv[1]);
    if (epsilon <= 0) {
        printf("Invalid input for epsilon: must be greater than zero.\n");
        return 1;
    }

    printf("Using epsilon: %.10f\n", epsilon);
    printf("---------------------------------------------------------\n");
    printf("Integral Function       | Result\n");
    printf("------------------------|------------------------\n");

    double result_a = integrate(f_a, 0.0, 1.0, epsilon);
    double result_b = integrate(f_b, 0.0, 1.0, epsilon);
    double result_c = integrate(f_c, 0.0, 0.999, epsilon);
    double result_d = integrate(f_d, 0.0, 1.0, epsilon);

    printf("Integral a (ln(1+x)/x)  | %.10f\n", result_a);
    printf("Integral b (exp(-x^2/2))| %.10f\n", result_b);
    printf("Integral c (ln(1/(1-x)))| %.10f\n", result_c);
    printf("Integral d (x^x)        | %.10f\n", result_d);

    printf("------------------------|------------------------\n");
    return 0;
}

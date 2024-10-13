#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

char check_of_flag(char* str) {
    if (str[0] != '-' && str[0] != '/') {
        return 0;
    }
    if (strlen(str) != 2) {
        return 0;
    }
    return str[1];
}

int is_number(const char* str) {
    if (str[0] == '\0') return 0;

    int has_decimal = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            continue;
        } else if (str[i] == '.') {
            if (has_decimal) {
                return 0;
            }
            has_decimal = 1;
        } else if (str[i] == '-') {
            if (i > 0) {
                return 0;
            }
        } else {
            return 0;
        }
    }
    return 1;
}

double my_atof(char* str) {
    return strtod(str, NULL);
}

void equation_solution(double a, double b, double c, double epsilon) {
    double D;
    printf("\nSolve for: a = %lf, b = %lf, c = %lf\n", a, b, c);

    if (fabs(a) < epsilon && fabs(b) < epsilon) {
        printf("Incorrect values for coefficients\n");
        return;
    }

    if (fabs(a) < epsilon) {
        if (fabs(b) > epsilon) {
            double solution = -c / b;
            if (fabs(solution) < epsilon) solution = 0.0;
            printf("One solution: %lf\n", solution);
        } else {
            printf("Incorrect values for coefficients\n");
        }
        return;
    }

    D = b * b - 4.0 * a * c;
    if (fabs(D) < epsilon) {
        double solution = -b / (2.0 * a);
        if (fabs(solution) < epsilon) solution = 0.0;
        printf("One solution: %lf\n", solution);
    } else if (D > epsilon) {
        double solution1 = (-b + sqrt(D)) / (2.0 * a);
        double solution2 = (-b - sqrt(D)) / (2.0 * a);
        if (fabs(solution1) < epsilon) solution1 = 0.0;
        if (fabs(solution2) < epsilon) solution2 = 0.0;
        printf("Two solutions: %lf and %lf\n", solution1, solution2);
    } else {
        printf("No real solutions\n");
    }
}

void flag_q(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Incorrect number of arguments for flag -q\n");
        return;
    }

    for (int i = 2; i < 6; i++) {
        if (!is_number(argv[i])) {
            printf("Invalid input: All parameters must be numbers.\n");
            return;
        }
    }

    double epsilon = my_atof(argv[2]);

    if (epsilon > 1.0) {
        printf("Epsilon must be less than or equal to 1.\n");
        return;
    }

    double cof[3];
    for (int i = 0; i < 3; i++) {
        cof[i] = my_atof(argv[3 + i]);
    }

    equation_solution(cof[0], cof[1], cof[2], epsilon);
    equation_solution(cof[0], cof[2], cof[1], epsilon);
    equation_solution(cof[1], cof[0], cof[2], epsilon);
    equation_solution(cof[1], cof[2], cof[0], epsilon);
    equation_solution(cof[2], cof[0], cof[1], epsilon);
    equation_solution(cof[2], cof[1], cof[0], epsilon);
}

void flag_m(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Incorrect number of arguments for flag -m\n");
        return;
    }

    for (int i = 2; i < 4; i++) {
        if (!is_number(argv[i])) {
            printf("Invalid input: Both parameters must be integers.\n");
            return;
        }
    }

    long long a = my_atof(argv[2]);
    long long b = my_atof(argv[3]);

    if (b == 0) {
        printf("The second number cannot be zero.\n");
        return;
    }

    if (a % b == 0) {
        printf("%lld is multiple of %lld\n", a, b);
    } else {
        printf("%lld is not multiple of %lld\n", a, b);
    }
}

void flag_t(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Incorrect number of arguments for flag -t\n");
        return;
    }

    for (int i = 2; i < 6; i++) {
        if (!is_number(argv[i])) {
            printf("Invalid input: All parameters must be numbers.\n");
            return;
        }
    }

    double epsilon = my_atof(argv[2]);
    double sides[3];

    for (int i = 0; i < 3; i++) {
        sides[i] = my_atof(argv[3 + i]);
        if (sides[i] <= 0) {
            printf("Side %d must be greater than zero.\n", i + 1);
            return;
        }
    }

    if (fabs(sides[0] * sides[0] - (sides[1] * sides[1] + sides[2] * sides[2])) < epsilon ||
        fabs(sides[1] * sides[1] - (sides[0] * sides[0] + sides[2] * sides[2])) < epsilon ||
        fabs(sides[2] * sides[2] - (sides[0] * sides[0] + sides[1] * sides[1])) < epsilon) {
        printf("Numbers can be sides of a right triangle\n");
    } else {
        printf("Numbers cannot be sides of a right triangle\n");
    }
}

int handle_flag(char op, int argc, char* argv[]) {
    switch (op) {
        case 'q':
            flag_q(argc, argv);
            break;
        case 'm':
            flag_m(argc, argv);
            break;
        case 't':
            flag_t(argc, argv);
            break;
        default:
            return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("No arguments provided\n");
        return 1;
    }

    char op = 0;

    for (int i = 1; i <= 2 && i < argc; i++) {
        op = check_of_flag(argv[i]);
        if (op) {
            if (i == 2) {
                char* temp = argv[1];
                argv[1] = argv[2];
                argv[2] = temp;
            }
            break;
        }
    }

    if (!op || !handle_flag(op, argc, argv)) {
        printf("Invalid flag or incorrect arguments\n");
        return 1;
    }

    return 0;
}

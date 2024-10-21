#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_BASE = -1,
    ERROR_INVALID_FRACTION = -2,
    ERROR_MEMORY_ALLOCATION = -3
} StatusCode;

StatusCode acount(const int base, const double EPS, const int count, ...)
{
    if (base <= 1) {
        return ERROR_INVALID_BASE;
    }

    va_list vl;
    va_start(vl, count);

    for (int i = 0; i < count; i++) {
        double d = va_arg(vl, double);

        if (d <= 0.0 || d >= 1.0) {
            printf("Error: %lf is out of range (0, 1)\n", d);
            va_end(vl);
            return ERROR_INVALID_FRACTION;
        }

        printf("%lf is ", d);

        double* arr = (double*)malloc(100 * sizeof(double));
        if (arr == NULL) {
            printf("Memory allocation error\n");
            va_end(vl);
            return ERROR_MEMORY_ALLOCATION;
        }

        int end = 0;
        int finite = 1;
        double whole_part;
        
        arr[end++] = d;

        while (d > EPS && finite) {
            d *= base;
            d = modf(d, &whole_part);

            for (int j = 0; j < end; j++) {
                if (fabs(d - arr[j]) < EPS) {
                    printf("infinite\n");
                    finite = 0;
                    break;
                }
            }
            arr[end++] = d;
        }

        if (finite) {
            printf("finite\n");
        }

        free(arr);
    }

    va_end(vl);
    return SUCCESS;
}

int main()
{
    int base;
    const double EPS = 0.0000001;

    printf("base of system = ");
    if (scanf("%d", &base) != 1 || base <= 1) {
        printf("Invalid input for base. Base must be an integer greater than 1.\n");
        return ERROR_INVALID_BASE;
    }

    StatusCode result = acount(base, EPS, 5, 1.0/3.0, 4.0/10.0, 0.3, 1.0/7.0, 0.45);

    if (result != SUCCESS) {
        switch (result) {
            case ERROR_INVALID_BASE:
                printf("Error: invalid base\n");
                break;
            case ERROR_INVALID_FRACTION:
                printf("Error: invalid fraction\n");
                break;
            case ERROR_MEMORY_ALLOCATION:
                printf("Error: memory allocation failed\n");
                break;
            default:
                printf("Unknown error occurred\n");
                break;
        }
        return result;
    }

    return SUCCESS; 
}

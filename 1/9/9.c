#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

void fill_array(long long *arr, long long size, long long a, long long b) {
    for (long long i = 0; i < size; i++) {
        arr[i] = rand() % (b - a + 1) + a;
    }
}

void swap_min_max(long long *arr, long long size) {
    long long min_index = 0, max_index = 0;

    for (long long i = 1; i < size; i++) {
        if (arr[i] < arr[min_index]) {
            min_index = i;
        }
        if (arr[i] > arr[max_index]) {
            max_index = i;
        }
    }

    if (min_index != max_index) {
        long long temp = arr[min_index];
        arr[min_index] = arr[max_index];
        arr[max_index] = temp;
    }
}

int compare(const void *a, const void *b) {
    return (*(long long *)a - *(long long *)b);
}

long long binary_search_nearest(long long value, long long *arr, long long size) {
    long long low = 0;
    long long high = size - 1;
    long long nearest_index = 0;
    long long min_difference = llabs(arr[0] - value);
    
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        long long difference = llabs(arr[mid] - value);

        if (difference < min_difference) {
            min_difference = difference;
            nearest_index = mid;
        }

        if (arr[mid] < value) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return arr[nearest_index];
}

void print_array(long long *arr, long long size, const char *name) {
    printf("%s:\n", name);
    long long limit = 100;
    for (long long i = 0; i < size; i++) {
        printf("%lld ", arr[i]);
        if (i >= limit - 1 && i < size - 1) {
            printf("\n... (total %lld elements)\n", size);
            break;
        }
    }
    printf("\n");
}

long long safe_strtoll(const char *str) {
    char *endptr;
    long long val = strtoll(str, &endptr, 10);

    if (*endptr != '\0') {
        printf("Error: invalid input, not a number: %s\n", str);
        return LLONG_MIN;
    }

    return val;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    long long a = safe_strtoll(argv[1]);
    long long b = safe_strtoll(argv[2]);

    if (a == LLONG_MIN || b == LLONG_MIN) {
        return 1;
    }

    srand(time(NULL));

    long long fixed_size = 10;
    long long *fixed_array = (long long *)malloc(fixed_size * sizeof(long long));
    fill_array(fixed_array, fixed_size, a, b);

    print_array(fixed_array, fixed_size, "Fixed array");

    swap_min_max(fixed_array, fixed_size);

    print_array(fixed_array, fixed_size, "Array after swapping min and max");

    free(fixed_array);

    long long size_A = rand() % (10000 - 10 + 1) + 10;
    long long size_B = rand() % (10000 - 10 + 1) + 10;
    long long *array_A = (long long *)malloc(size_A * sizeof(long long));
    long long *array_B = (long long *)malloc(size_B * sizeof(long long));
    
    fill_array(array_A, size_A, -1000, 1000);
    fill_array(array_B, size_B, -1000, 1000);

    qsort(array_B, size_B, sizeof(long long), compare);

    long long *array_C = (long long *)malloc(size_A * sizeof(long long));
    for (long long i = 0; i < size_A; i++) {
        array_C[i] = array_A[i] + binary_search_nearest(array_A[i], array_B, size_B);

        if (array_C[i] > LLONG_MAX || array_C[i] < LLONG_MIN) {
            printf("Error: overflow occurred during calculations\n");
            free(array_A);
            free(array_B);
            free(array_C);
            return 1;
        }
    }

    print_array(array_A, size_A, "Array A");
    print_array(array_B, size_B, "Array B");
    print_array(array_C, size_A, "Array C");

    free(array_A);
    free(array_B);
    free(array_C);

    return 0;
}

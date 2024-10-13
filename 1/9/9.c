#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void fill_array(int *arr, int size, int a, int b) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (b - a + 1) + a;
    }
}

void swap_min_max(int *arr, int size) {
    int min_index = 0, max_index = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[min_index]) {
            min_index = i;
        }
        if (arr[i] > arr[max_index]) {
            max_index = i;
        }
    }

    if (min_index != max_index) {
        int temp = arr[min_index];
        arr[min_index] = arr[max_index];
        arr[max_index] = temp;
    }
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int binary_search_nearest(int value, int *arr, int size) {
    int low = 0;
    int high = size - 1;
    int nearest_index = 0;
    int min_difference = abs(arr[0] - value);
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int difference = abs(arr[mid] - value);

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

void print_array(int *arr, int size, const char *name) {
    printf("%s:\n", name);
    int limit = 100;
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
        if (i >= limit - 1 && i < size - 1) {
            printf("\n... (total %d elements)\n", size);
            break;
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    srand(time(NULL));

    int fixed_size = 10;
    int *fixed_array = (int *)malloc(fixed_size * sizeof(int));
    fill_array(fixed_array, fixed_size, a, b);

    print_array(fixed_array, fixed_size, "Fixed array");

    swap_min_max(fixed_array, fixed_size);

    print_array(fixed_array, fixed_size, "Array after swapping min and max");

    free(fixed_array);

    int size_A = rand() % (10000 - 10 + 1) + 10;
    int size_B = rand() % (10000 - 10 + 1) + 10;
    int *array_A = (int *)malloc(size_A * sizeof(int));
    int *array_B = (int *)malloc(size_B * sizeof(int));
    
    fill_array(array_A, size_A, -1000, 1000);
    fill_array(array_B, size_B, -1000, 1000);

    qsort(array_B, size_B, sizeof(int), compare);

    int *array_C = (int *)malloc(size_A * sizeof(int));
    for (int i = 0; i < size_A; i++) {
        array_C[i] = array_A[i] + binary_search_nearest(array_A[i], array_B, size_B);
    }

    print_array(array_A, size_A, "Array A");
    print_array(array_B, size_B, "Array B");
    print_array(array_C, size_A, "Array C");

    free(array_A);
    free(array_B);
    free(array_C);

    return 0;
}

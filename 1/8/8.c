#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int get_min_base(const char* number) {
    int min_base = 2; 
    const char* p = number;

    if (*p == '-') {
        p++;
    }

    for (; *p; ++p) {
        int value;
        if (isdigit(*p)) {
            value = *p - '0'; 
        } else if (isalpha(*p)) {
            value = tolower(*p) - 'a' + 10; 
        } else {
            return -1; 
        }
        if (value + 1 > min_base) {
            min_base = value + 1;
        }
    }
    return min_base;
}

int convert_to_decimal(const char* number, int base, long long* result) {
    long long temp_result = 0;
    int is_negative = 0;

    if (*number == '-') {
        is_negative = 1;
        number++;
    }

    for (const char* p = number; *p; ++p) {
        temp_result *= base;
        if (temp_result > LONG_MAX / base) {
            return -1;
        }
        if (isdigit(*p)) {
            temp_result += *p - '0'; 
        } else {
            temp_result += tolower(*p) - 'a' + 10; 
        }
        if (temp_result < 0) {
            return -1;
        }
    }

    *result = is_negative ? -temp_result : temp_result; 
    return 0;
}

void process_file(const char* input_path, const char* output_path) {
    if (strcmp(input_path, output_path) == 0) {
        FILE* output_file = fopen(output_path, "a");
        if (output_file) {
            fprintf(output_file, "Error: Input and output files must be different.\n");
            fclose(output_file);
        }
        return;
    }

    FILE* input_file = fopen(input_path, "r");
    FILE* output_file = fopen(output_path, "w");
    
    if (!input_file || !output_file) {
        if (output_file) fclose(output_file);
        FILE* output_file_err = fopen(output_path, "a");
        if (output_file_err) {
            perror("Error opening file");
            fclose(output_file_err);
        }
        return;
    }

    char buffer[100];
    while (fscanf(input_file, "%99s", buffer) == 1) {
        char* trimmed_number = buffer;

        if (trimmed_number[0] == '-') {
            trimmed_number++;
            while (*trimmed_number == '0' && *(trimmed_number + 1) != '\0') {
                trimmed_number++;
            }
            trimmed_number--;
        } else {
            while (*trimmed_number == '0' && *(trimmed_number + 1) != '\0') {
                trimmed_number++;
            }
        }

        int min_base = get_min_base(trimmed_number);
        if (min_base < 2) {
            fprintf(output_file, "Invalid number: %s\n", trimmed_number);
            continue; 
        }

        long long decimal_value;
        if (convert_to_decimal(trimmed_number, min_base, &decimal_value) != 0) {
            fprintf(output_file, "Overflow: %s\n", trimmed_number);
            continue; 
        }
        
        fprintf(output_file, "%s %d %lld\n", trimmed_number, min_base, decimal_value);
    }

    fclose(input_file);
    fclose(output_file);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        FILE* output_file = fopen(argv[2], "a");
        if (output_file) {
            fprintf(output_file, "Usage: %s <input_file> <output_file>\n", argv[0]);
            fclose(output_file);
        }
        return 1;
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        FILE* output_file = fopen(argv[2], "a");
        if (output_file) {
            fprintf(output_file, "Error: Input and output files must be different.\n");
            fclose(output_file);
        }
        return 1;
    }

    process_file(argv[1], argv[2]);
    return 0;
}

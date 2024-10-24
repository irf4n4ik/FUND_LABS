#define _CRT_SECURE_NO_WARNINGS
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compute_prefix_function(const char* pattern, int* prefix, int len_sub) {
    int length = 0;
    prefix[0] = 0;

    for (int i = 1; i < len_sub; i++) {
        while (length > 0 && pattern[i] != pattern[length]) {
            length = prefix[length - 1];
        }
        if (pattern[i] == pattern[length]) {
            length++;
        }
        prefix[i] = length;
    }
}

void Search_In_File(const char* substring, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File %s isn't found!\n", filename);
        return;
    }

    int len_sub = strlen(substring);
    if (len_sub == 0) {
        printf("Substring is empty!\n");
        fclose(file);
        return;
    }

    int* prefix = (int*)malloc(len_sub * sizeof(int));
    if (!prefix) {
        printf("Memory allocation failed for prefix array!\n");
        fclose(file);
        return;
    }

    compute_prefix_function(substring, prefix, len_sub);

    int line_number = 1;
    int match_count = 0;
    int position_in_line = 1;
    int j = 0;

    char c;

    printf("Searching in file: %s\n", filename);

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            line_number++;
            position_in_line = 1;
            j = 0;
            continue;
        }

        while (j > 0 && c != substring[j]) {
            j = prefix[j - 1];
        }
        if (c == substring[j]) {
            j++;
        }
        if (j == len_sub) {
            match_count++;
            printf("Match #%d found at line %d, position %d\n", match_count, line_number, position_in_line - len_sub + 1);
            j = prefix[j - 1];
        }

        position_in_line++;
    }

    if (match_count == 0) {
        printf("No matches found in file %s\n", filename);
    }

    fclose(file);
    free(prefix);
}

void Search_from_files(int num_of_files, ...) {
    va_list file_list;
    va_start(file_list, num_of_files);

    const char* substring = va_arg(file_list, const char*);

    for (int i = 1; i < num_of_files; i++) {
        const char* filename = va_arg(file_list, const char*);
        Search_In_File(substring, filename);
    }

    va_end(file_list);
}

int main() {
    Search_from_files(4, "test", "1.txt", "2.txt", "3.txt");

    return 0;
}

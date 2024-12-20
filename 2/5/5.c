﻿#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 100
#define MAX_FIB 32
#define MAX_OUTPUT 500

typedef enum {
    SUCCESS = 0,
    ERR_INVALID_BASE = 1,
    ERR_MEMORY_ALLOCATION = 2,
    ERR_FILE_OPEN = 3,
    ERR_INVALID_NUMBER = 4,
} StatusCode;

void my_strcat(char *dest, const char *src) {
    while (*dest) {
        dest++;
    }
    while ((*dest++ = *src++));
}

void int_to_roman(int num, char *buffer) {
    const char *roman_numerals[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    int i = 0;
    buffer[0] = '\0';

    while (num > 0) {
        while (num >= values[i]) {
            my_strcat(buffer, roman_numerals[i]);
            num -= values[i];
        }
        i++;
    }
}

void int_to_zeckendorf(unsigned int num, char *buffer) {
    unsigned int fib[MAX_FIB] = {1, 2};
    int count = 2;

    while (count < MAX_FIB && fib[count - 1] + fib[count - 2] <= num) {
        fib[count] = fib[count - 1] + fib[count - 2];
        count++;
    }

    buffer[0] = '\0';
    int used_fib = 0;
    for (int i = count - 1; i >= 0; i--) {
        if (num >= fib[i]) {
            num -= fib[i];
            my_strcat(buffer, "1");
            used_fib = 1;
        } else if (used_fib) {
            my_strcat(buffer, "0");
        }
    }
    my_strcat(buffer, "1");
}

void int_to_base(int num, int base, char *buffer, int uppercase) {
    const char *digits = uppercase ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";
    char temp[65];
    int index = 0;

    if (base < 2 || base > 36) {
        buffer[0] = '\0';
        return;
    }

    int negative = 0;
    if (num < 0) {
        negative = 1;
        num = -num;
    }

    do {
        temp[index++] = digits[num % base];
        num /= base;
    } while (num > 0);

    if (negative) {
        temp[index++] = '-';
    }

    for (int i = 0; i < index; i++) {
        buffer[i] = temp[index - 1 - i];
    }
    buffer[index] = '\0';
}

int base_to_decimal(const char *str, int base) {
    return (int)strtol(str, NULL, base);
}

int is_upper_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (islower(str[i])) {
            return 0;
        }
    }
    return 1;
}

void memory_dump_int(int num, char *buffer) {
    unsigned char *bytes = (unsigned char*)&num;
    for (int i = sizeof(num) - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            my_strcat(buffer, (bytes[i] & (1 << j)) ? "1" : "0");
        }
        my_strcat(buffer, " ");
    }
}

void memory_dump_unsigned_int(unsigned int num, char *buffer) {
    unsigned char *bytes = (unsigned char*)&num;
    for (int i = sizeof(num) - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            my_strcat(buffer, (bytes[i] & (1 << j)) ? "1" : "0");
        }
        my_strcat(buffer, " ");
    }
}

void memory_dump_double(double num, char *buffer) {
    unsigned char *bytes = (unsigned char*)&num;
    for (int i = sizeof(num) - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            my_strcat(buffer, (bytes[i] & (1 << j)) ? "1" : "0");
        }
        my_strcat(buffer, " ");
    }
}

void memory_dump_float(float num, char *buffer) {
    unsigned char *bytes = (unsigned char*)&num;
    for (int i = sizeof(num) - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            my_strcat(buffer, (bytes[i] & (1 << j)) ? "1" : "0");
        }
        my_strcat(buffer, " ");
    }
}

int oversprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char *s = str;

    while (*format) {
        if (*format == '%') {
            format++;
            if (*format == 'R' && *(format + 1) == 'o') {  // %Ro
                int num = va_arg(args, int);
                char buffer[BUFFER_SIZE];
                int_to_roman(num, buffer);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 'Z' && *(format + 1) == 'r') {  // %Zr
                unsigned int num = va_arg(args, unsigned int);
                char buffer[BUFFER_SIZE];
                int_to_zeckendorf(num, buffer);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 'C' && (*(format + 1) == 'v' || *(format + 1) == 'V')) {  // %Cv / %CV
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                char buffer[BUFFER_SIZE];
                int uppercase = (*(format + 1) == 'V') ? 1 : 0;
                int_to_base(num, base, buffer, uppercase);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 't' && *(format + 1) == 'o') {  // %to
                char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                int num = base_to_decimal(num_str, base);
                s += sprintf(s, "%d", num);
                format += 2;
            } else if (*format == 'T' && *(format + 1) == 'O') {  // %TO
                char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                if (!is_upper_string(num_str)) {
                    va_end(args);
                    return -1;
                }
                int num = base_to_decimal(num_str, base);
                s += sprintf(s, "%d", num);
                format += 2;
            } else if (*format == 'm' && *(format + 1) == 'i') {  // %mi
                int num = va_arg(args, int);
                char buffer[BUFFER_SIZE] = "";
                memory_dump_int(num, buffer);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 'm' && (*(format + 1) == 'u')) {  // %mu
                unsigned int num = va_arg(args, unsigned int);
                char buffer[BUFFER_SIZE] = "";
                memory_dump_unsigned_int(num, buffer);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 'm' && (*(format + 1) == 'd')) {  // %md
                double num = va_arg(args, double);
                char buffer[BUFFER_SIZE] = "";
                memory_dump_double(num, buffer);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 'm' && (*(format + 1) == 'f')) {  // %mf
                float num = va_arg(args, double);
                char buffer[BUFFER_SIZE] = "";
                memory_dump_float(num, buffer);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else {
                *s++ = '%';
            }
        } else {
            *s++ = *format++;
        }
    }

    *s = '\0';
    va_end(args);

    return strlen(str);
}

int overfprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            if (*format == 'R' && *(format + 1) == 'o') {  // %Ro
                int num = va_arg(args, int);
                char buffer[BUFFER_SIZE];
                int_to_roman(num, buffer);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 'Z' && *(format + 1) == 'r') {  // %Zr
                unsigned int num = va_arg(args, unsigned int);
                char buffer[BUFFER_SIZE];
                int_to_zeckendorf(num, buffer);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 'C' && (*(format + 1) == 'v' || *(format + 1) == 'V')) {  // %Cv / %CV
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                char buffer[BUFFER_SIZE];
                int uppercase = (*(format + 1) == 'V') ? 1 : 0;
                int_to_base(num, base, buffer, uppercase);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 't' && *(format + 1) == 'o') {  // %to
                char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                int num = base_to_decimal(num_str, base);
                fprintf(stream, "%d", num);
                format += 2;
            } else if (*format == 'T' && *(format + 1) == 'O') {  // %TO
                char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                if (!is_upper_string(num_str)) {
                    va_end(args);
                    return -1;
                }
                int num = base_to_decimal(num_str, base);
                fprintf(stream, "%d", num);
                format += 2;
            } else if (*format == 'm' && *(format + 1) == 'i') {  // %mi
                int num = va_arg(args, int);
                char buffer[BUFFER_SIZE] = "";
                memory_dump_int(num, buffer);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 'm' && (*(format + 1) == 'u')) {  // %mu
                unsigned int num = va_arg(args, unsigned int);
                char buffer[BUFFER_SIZE] = "";
                memory_dump_unsigned_int(num, buffer);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 'm' && (*(format + 1) == 'd')) {  // %md
                double num = va_arg(args, double);
                char buffer[BUFFER_SIZE] = "";
                memory_dump_double(num, buffer);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 'm' && (*(format + 1) == 'f')) {  // %mf
                float num = va_arg(args, double);
                char buffer[BUFFER_SIZE] = "";
                memory_dump_float(num, buffer);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else {
                fputc('%', stream);
            }
        } else {
            fputc(*format++, stream);
        }
    }

    va_end(args);

    return 0;
}

int main() {
    char buffer[MAX_OUTPUT];

    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return ERR_FILE_OPEN;
    }

    // %Ro
    overfprintf(file, "Roman representation of 1024: %Ro\n", 1024);
    oversprintf(buffer, "Roman representation of 1024: %Ro\n", 1024);
    printf("%s", buffer);

    // %Zr
    overfprintf(file, "Zeckendorf representation of 88: %Zr\n", 88);
    oversprintf(buffer, "Zeckendorf representation of 88: %Zr\n", 88);
    printf("%s", buffer);

    // %Cv / %CV
    overfprintf(file, "Number 255 in base 16: %Cv\n", 255, 16);
    oversprintf(buffer, "Number 255 in base 16: %Cv\n", 255, 16);
    printf("%s", buffer);

    overfprintf(file, "Number 255 in base 16 (uppercase): %CV\n", 255, 16);
    oversprintf(buffer, "Number 255 in base 16 (uppercase): %CV\n", 255, 16);
    printf("%s", buffer);

    // %to
    overfprintf(file, "Converting string 2222 from binary to decimal: %to\n", "2222", 2);
    oversprintf(buffer, "Converting string 2222 from binary to decimal: %to\n", "2222", 2);
    printf("%s", buffer);

    // %TO
    overfprintf(file, "Converting string ABC from base 16 to decimal: %TO\n", "ABC", 16);
    oversprintf(buffer, "Converting string ABC from base 16 to decimal: %TO\n", "ABC", 16);
    printf("%s", buffer);

    // %mi
    overfprintf(file, "Memory dump of number 23: %mi\n", 23);
    oversprintf(buffer, "Memory dump of number 23: %mi\n", 23);
    printf("%s", buffer);

    fclose(file);

    return SUCCESS;
}

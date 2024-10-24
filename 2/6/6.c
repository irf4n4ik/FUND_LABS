#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define MAX_OUTPUT 1024

typedef enum {
    SUCCESS = 0,
    OVERFLOW_ERROR = 1,
    INPUT_FILE_ERROR = 2,
    NUMERAL_SYSTEM_ERROR = 3,
    ERR_FILE_OPEN = -1
} StatusCode;

int SizeString(const char *s) {
    return strlen(s);
}

StatusCode RomanTOInt(char *s, int *res) {
    int map[256] = {0};
    map['I'] = 1;
    map['V'] = 5;
    map['X'] = 10;
    map['L'] = 50;
    map['C'] = 100;
    map['D'] = 500;
    map['M'] = 1000;
    *res = 0;
    for (int i = 0; i < SizeString(s); ++i) {
        if (map[(int)s[i]] < map[(int)s[i + 1]]) {
            if (labs(*res - map[(int)s[i]]) >= INT_MAX) return OVERFLOW_ERROR;
            *res -= map[(int)s[i]];
        } else {
            if (labs(*res + map[(int)s[i]]) >= INT_MAX) return OVERFLOW_ERROR;
            *res += map[(int)s[i]];
        }
    }
    return SUCCESS;
}

StatusCode ZeckendorfTOUInt(char *s, unsigned int *res) {
    unsigned int fib[33];
    *res = 0;
    fib[0] = 1;
    fib[1] = 1;
    for (int i = 2; i < 33; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    for (int i = 0; s[i]; ++i) {
        if (s[i] == '1') {
            if ((*res + fib[i]) >= INT_MAX) return OVERFLOW_ERROR;
            *res += fib[i];
        }
    }
    return SUCCESS;
}

int sequence_number(char c) {
    if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 10;
    else if (c >= '0' && c <= '9')
        return c - '0';
    else
        return -1;
}

StatusCode FromXTo10(char *original, int based, int *result) {
    *result = 0;
    int fl = 0;
    for (int i = 0; i < SizeString(original); ++i) {
        if (original[i] == '-')
            fl = 1;
        else {
            if (sequence_number(original[i]) == -1 || sequence_number(original[i]) >= based)
                return NUMERAL_SYSTEM_ERROR;
            *result = (*result) * based + sequence_number(original[i]);
        }
    }
    if (fl) *result *= -1;
    return SUCCESS;
}

int IsNotUpper(char c) {
    return (c < 'A' || c > 'Z');
}

int IsNotLow(char c) {
    return (c < 'a' || c > 'z');
}

// overfscanf
int overfscanf(FILE *restrict stream, char *format, ...) {
    va_list factor;
    StatusCode errorMsg;
    va_start(factor, format);
    if (!stream) {
        return INPUT_FILE_ERROR;
    }
    int count = 0;
    const char *p = format;
    while (*p) {
        if (*p == '%' && *(p + 1) != '\0') {
            ++p;
            if (*p == '%') {
                ++p;
            } else if (SizeString(p) >= 2 && *p == 'R' && *(p + 1) == 'o') {
                int *res = va_arg(factor, int *);
                int i = 0;
                char input_char;
                char input_str[100];
                while ((input_char = getc(stream)) != EOF &&
                       (input_char == 'M' || input_char == 'D' || input_char == 'C' || input_char == 'L' ||
                        input_char == 'V' || input_char == 'I' || input_char == 'X')) {
                    input_str[i++] = input_char;
                }
                fseek(stream, -1, SEEK_CUR);
                input_str[i] = '\0';
                errorMsg = RomanTOInt(input_str, res);
                if (errorMsg) {
                    va_end(factor);
                    return count;
                }
                count++;
                p += 2;
            } else if (SizeString(p) >= 2 && *p == 'Z' && *(p + 1) == 'r') {
                unsigned int *res = va_arg(factor, unsigned int *);
                char input_char;
                char input_str[50];
                int i = 0;
                while ((input_char = getc(stream)) != EOF && (input_char == '0' || input_char == '1')) {
                    input_str[i++] = input_char;
                }
                errorMsg = ZeckendorfTOUInt(input_str, res);
                fseek(stream, -1, SEEK_CUR);
                if (errorMsg) {
                    va_end(factor);
                    return count;
                }
                count++;
                p += 2;
            } else if (SizeString(p) >= 2 && *p == 'C' && *(p + 1) == 'v') {
                int *res = va_arg(factor, int *);
                int base = va_arg(factor, int);
                if (base < 2 || base > 36) base = 10;
                *res = 0;
                char str[100], input_char;
                int i = 0;
                while (((input_char = getc(stream)) != EOF && sequence_number(input_char) != -1 &&
                        sequence_number(input_char) < base && IsNotUpper(input_char)) || input_char == '-') {
                    str[i++] = input_char;
                }
                str[i] = '\0';
                errorMsg = FromXTo10(str, base, res);
                if (errorMsg || i == 0 || (str[0] == '-' && i == 1)) {
                    va_end(factor);
                    return count;
                }
                count += 1;
                fseek(stream, -1, SEEK_CUR);
                p += 2;
            } else if (SizeString(p) >= 2 && (*p == 'C' && *(p + 1) == 'V')) {
                int *res = va_arg(factor, int *);
                int base = va_arg(factor, int);
                if (base < 2 || base > 36) base = 10;
                *res = 0;
                char str[100], input_char;
                int i = 0;
                while (((input_char = getc(stream)) && sequence_number(input_char) != -1 &&
                        sequence_number(input_char) < base && IsNotLow(input_char)) || input_char == '-') {
                    str[i++] = input_char;
                }
                str[i] = '\0';
                errorMsg = FromXTo10(str, base, res);
                if (errorMsg || i == 0 || (str[0] == '-' && i == 1)) {
                    va_end(factor);
                    return count;
                }
                count += 1;
                fseek(stream, -1, SEEK_CUR);
                p += 2;
            } else {
                char tmp[100];
                strncpy(tmp, p, sizeof(tmp) - 1);
                tmp[sizeof(tmp) - 1] = '\0';
                int n = vfscanf(stream, tmp, factor);
                va_arg(factor, void *);
                if (n == 0) {
                    va_end(factor);
                    return count;
                }
                count += n;
                --p;
            }
        } else {
            char c = getc(stream);
            if (c != *p || feof(stream)) {
                va_end(factor);
                return count;
            }
            p++;
        }
    }
    va_end(factor);
    return count;
}

// oversscanf
int oversscanf(char *buffer, char *format, ...) {
    va_list factor;
    StatusCode errorMsg;
    va_start(factor, format);
    int count = 0;
    const char *p = format;
    while (*p) {
        if (*p == '%' && *(p + 1) != '\0') {
            ++p;
            if (*p == '%') {
                ++p;
            } else if (SizeString(p) >= 2 && *p == 'R' && *(p + 1) == 'o') {
                int *res = va_arg(factor, int *);
                int i = 0;
                char input_str[100];
                while (buffer && *buffer && (*buffer == 'M' || *buffer == 'D' || *buffer == 'C' || *buffer == 'L' ||
                                             *buffer == 'V' || *buffer == 'I' || *buffer == 'X')) {
                    input_str[i++] = *buffer;
                    buffer++;
                }
                input_str[i] = '\0';
                errorMsg = RomanTOInt(input_str, res);
                if (errorMsg) {
                    va_end(factor);
                    return count;
                }
                count++;
                p += 2;
            } else if (SizeString(p) >= 2 && *p == 'Z' && *(p + 1) == 'r') {
                unsigned int *res = va_arg(factor, unsigned int *);
                char input_str[50];
                int i = 0;
                while (buffer && *buffer && (*buffer == '0' || *buffer == '1')) {
                    input_str[i++] = *buffer;
                    buffer++;
                }
                input_str[i] = '\0';
                errorMsg = ZeckendorfTOUInt(input_str, res);
                if (errorMsg) {
                    va_end(factor);
                    return count;
                }
                count++;
                p += 2;
            } else if (SizeString(p) >= 2 && *p == 'C' && *(p + 1) == 'v') {
                int *res = va_arg(factor, int *);
                int base = va_arg(factor, int);
                if (base < 2 || base > 36) base = 10;
                *res = 0;
                char str[100];
                int i = 0;
                while ((*buffer && sequence_number(*buffer) != -1 && sequence_number(*buffer) < base && IsNotUpper(*buffer)) ||
                       *buffer == '-') {
                    str[i++] = *buffer;
                    buffer++;
                }
                str[i] = '\0';
                errorMsg = FromXTo10(str, base, res);
                if (errorMsg || i == 0 || (str[0] == '-' && i == 1)) {
                    va_end(factor);
                    return count;
                }
                count += 1;
                p += 2;
            } else if (SizeString(p) >= 2 && (*p == 'C' && *(p + 1) == 'V')) {
                int *res = va_arg(factor, int *);
                int base = va_arg(factor, int);
                if (base < 2 || base > 36) base = 10;
                *res = 0;
                char str[100];
                int i = 0;
                while ((*buffer && sequence_number(*buffer) != -1 && sequence_number(*buffer) < base && IsNotLow(*buffer)) ||
                       *buffer == '-') {
                    str[i++] = *buffer;
                    buffer++;
                }
                str[i] = '\0';
                errorMsg = FromXTo10(str, base, res);
                if (errorMsg || i == 0 || (str[0] == '-' && i == 1)) {
                    va_end(factor);
                    return count;
                }
                count += 1;
                p += 2;
            } else {
                char tmp[100];
                strncpy(tmp, p, sizeof(tmp) - 1);
                tmp[sizeof(tmp) - 1] = '\0';
                int n = vsscanf(buffer, tmp, factor);
                va_arg(factor, void *);
                if (n == 0) {
                    va_end(factor);
                    return count;
                }
                count += n;
                --p;
            }
        } else {
            if (*buffer != *p || *buffer == '\0') {
                va_end(factor);
                return count;
            }
            p++;
            buffer++;
        }
    }
    va_end(factor);
    return count;
}


int main() {
    char buffer[] = "MCMXCIV 110101 19 AB";
    int roman_number, base10_number, hex_number;
    unsigned int zeckendorf_number;

    // oversscanf
    if (oversscanf(buffer, "%Ro %Zr %Cv %Cv", &roman_number, &zeckendorf_number, &base10_number, 10, &hex_number, 16) > 0) {
        printf("Output from buffer:\n");
        printf("-------------------\n");
        printf("Roman numeral: %d\n", roman_number);
        printf("Zeckendorf representation: %u\n", zeckendorf_number);
        printf("Number in base 10: %d\n", base10_number);
        printf("Hexadecimal number: %d\n", hex_number);
        printf("-------------------\n\n");
    } else {
        printf("Failed to parse input from buffer.\n");
    }

    // overfscanf
    FILE *file = fopen("input.txt", "r");
    if (file) {
        if (overfscanf(file, "%Ro %Zr %Cv %Cv", &roman_number, &zeckendorf_number, &base10_number, 10, &hex_number, 16) > 0) {
            printf("Output from file:\n");
            printf("-------------------\n");
            printf("Roman numeral: %d\n", roman_number);
            printf("Zeckendorf representation: %u\n", zeckendorf_number);
            printf("Number in base 10: %d\n", base10_number);
            printf("Hexadecimal number: %d\n", hex_number);
            printf("-------------------\n");
        } else {
            printf("Failed to parse input from file.\n");
        }
        fclose(file);
    } else {
        printf("Error opening the file!\n");
    }

    return 0;
}
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void int_to_roman(int num, char *buffer) {
    struct roman_numeral {
        int value;
        const char *symbol;
    };
    
    struct roman_numeral roman[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };
    
    int i = 0;
    buffer[0] = '\0';
    
    while (num > 0) {
        if (num >= roman[i].value) {
            strcat(buffer, roman[i].symbol);
            num -= roman[i].value;
        } else {
            i++;
        }
    }
}

void int_to_zeckendorf(unsigned int num, char *buffer) {
    int fib[32];
    fib[0] = 1;
    fib[1] = 2;
    int count = 2;
    while (fib[count - 1] + fib[count - 2] <= num) {
        fib[count] = fib[count - 1] + fib[count - 2];
        count++;
    }
    
    buffer[0] = '\0';
    
    int used_fib = 0;
    for (int i = count - 1; i >= 0; i--) {
        if (num >= fib[i]) {
            num -= fib[i];
            strcat(buffer, "1");
            used_fib = 1;
        } else if (used_fib) {
            strcat(buffer, "0");
        }
    }
    strcat(buffer, "1");
}

void int_to_base(int num, int base, char *buffer, int uppercase) {
    char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char temp[65];
    int index = 0;
    
    if (uppercase) {
        for (int i = 10; i < 36; i++) {
            digits[i] = toupper(digits[i]);
        }
    }
    
    if (base < 2 || base > 36) base = 10;
    
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
    
    temp[index] = '\0';
    
    for (int i = 0; i < index; i++) {
        buffer[i] = temp[index - 1 - i];
    }
    buffer[index] = '\0';
}

int base_to_decimal(const char *str, int base) {
    return strtol(str, NULL, base);
}

void memory_dump_int(int num, char *buffer) {
    unsigned char *bytes = (unsigned char*)&num;
    for (int i = sizeof(num) - 1; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
            strcat(buffer, (bytes[i] & (1 << j)) ? "1" : "0");
        }
        strcat(buffer, " ");
    }
}

int oversprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    char *s = str;
    
    while (*format) {
        if (*format == '%') {
            format++;
            if (*format == 'R' && *(format + 1) == 'o') {  //%Ro
                int num = va_arg(args, int);
                char buffer[100];
                int_to_roman(num, buffer);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 'Z' && *(format + 1) == 'r') {  //%Zr
                unsigned int num = va_arg(args, unsigned int);
                char buffer[100];
                int_to_zeckendorf(num, buffer);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 'C' && (*(format + 1) == 'v' || *(format + 1) == 'V')) {  //%Cv / %CV
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                char buffer[100];
                int uppercase = (*(format + 1) == 'V') ? 1 : 0;
                int_to_base(num, base, buffer, uppercase);
                s += sprintf(s, "%s", buffer);
                format += 2;
            } else if (*format == 't' && *(format + 1) == 'o') {  //%to
                char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                int num = base_to_decimal(num_str, base);
                s += sprintf(s, "%d", num);
                format += 2;
            } else if (*format == 'm' && (*(format + 1) == 'i' || *(format + 1) == 'u')) {  //%mi / %mu
                int num = va_arg(args, int);
                char buffer[100] = "";
                memory_dump_int(num, buffer);
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
            if (*format == 'R' && *(format + 1) == 'o') {  //%Ro
                int num = va_arg(args, int);
                char buffer[100];
                int_to_roman(num, buffer);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 'Z' && *(format + 1) == 'r') {  //%Zr
                unsigned int num = va_arg(args, unsigned int);
                char buffer[100];
                int_to_zeckendorf(num, buffer);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 'C' && (*(format + 1) == 'v' || *(format + 1) == 'V')) {  //%Cv / %CV
                int num = va_arg(args, int);
                int base = va_arg(args, int);
                char buffer[100];
                int uppercase = (*(format + 1) == 'V') ? 1 : 0;
                int_to_base(num, base, buffer, uppercase);
                fprintf(stream, "%s", buffer);
                format += 2;
            } else if (*format == 't' && *(format + 1) == 'o') {  //%to
                char *num_str = va_arg(args, char *);
                int base = va_arg(args, int);
                int num = base_to_decimal(num_str, base);
                fprintf(stream, "%d", num);
                format += 2;
            } else if (*format == 'm' && (*(format + 1) == 'i' || *(format + 1) == 'u')) {  //%mi / %mu
                int num = va_arg(args, int);
                char buffer[100] = "";
                memory_dump_int(num, buffer);
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
    char buffer[500];

    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    //%Ro
    overfprintf(file, "Roman representation of 2024: %Ro\n", 2024);
    oversprintf(buffer, "Roman representation of 2024: %Ro\n", 2024);
    printf("%s", buffer);
    
    //%Zr
    overfprintf(file, "Zeckendorf representation of 52: %Zr\n", 52);
    oversprintf(buffer, "Zeckendorf representation of 52: %Zr\n", 52);
    printf("%s", buffer);
    
    //%Cv / %CV
    overfprintf(file, "Number 255 in base 16: %Cv\n", 255, 16);
    oversprintf(buffer, "Number 255 in base 16: %Cv\n", 255, 16);
    printf("%s", buffer);
    
    overfprintf(file, "Number 255 in base 16 (uppercase): %CV\n", 255, 16);
    oversprintf(buffer, "Number 255 in base 16 (uppercase): %CV\n", 255, 16);
    printf("%s", buffer);
    
    //%to
    overfprintf(file, "Converting string 1111 from binary to decimal: %to\n", "1111", 2);
    oversprintf(buffer, "Converting string 1111 from binary to decimal: %to\n", "1111", 2);
    printf("%s", buffer);
    
    //%mi
    overfprintf(file, "Memory dump of number 42: %mi\n", 42);
    oversprintf(buffer, "Memory dump of number 42: %mi\n", 42);
    printf("%s", buffer);

    fclose(file);
    
    return 0;
}

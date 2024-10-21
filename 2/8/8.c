#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    SUCCESS,
    MEMORY_ALLOCATION_ERROR,
    INVALID_NUMBER_ERROR,
    INVALID_BASE_ERROR,
    INVALID_ARGUMENT_COUNT_ERROR
} StatusCode;

char* check_str(const char* number, int len) {
    char* d;
    int diff = len - strlen(number);
    if (diff > 0) {
        d = (char*)malloc(sizeof(char) * (len + 2));
        if (!d) return NULL;
        for (int i = 0; i < diff; ++i) d[i] = '0';
        strcpy(d + diff, number);
    } else {
        d = (char*)malloc(sizeof(char) * (strlen(number) + 2));
        if (!d) return NULL;
        strcpy(d, number);
    }
    return d;
}

char* check_0(char* withzero) {
    if (withzero[0] != '0') return withzero;
    char* withoutzero = (char*)malloc(sizeof(char) * (strlen(withzero) + 1));
    if (!withoutzero) return NULL;
    strcpy(withoutzero, withzero + 1);
    free(withzero);
    return withoutzero;
}

StatusCode sum(const char* firstnum, const char* secondnum, int base, char** result) {
    int i, carry = 0;
    int maxlen = (strlen(firstnum) > strlen(secondnum)) ? strlen(firstnum) : strlen(secondnum);
    char* res = (char*)malloc(sizeof(char) * (maxlen + 2));
    if (!res) return MEMORY_ALLOCATION_ERROR;
    res[maxlen + 1] = '\0';

    char* firstnum_fixed = check_str(firstnum, maxlen);
    char* secondnum_fixed = check_str(secondnum, maxlen);
    if (!firstnum_fixed || !secondnum_fixed) {
        free(res);
        free(firstnum_fixed);
        free(secondnum_fixed);
        return MEMORY_ALLOCATION_ERROR;
    }

    for (i = 0; i < maxlen; ++i) {
        char num1 = firstnum_fixed[maxlen - i - 1];
        char num2 = secondnum_fixed[maxlen - i - 1];
        int digit1 = isdigit(num1) ? num1 - '0' : toupper(num1) - 'A' + 10;
        int digit2 = isdigit(num2) ? num2 - '0' : toupper(num2) - 'A' + 10;
        int sum = carry + digit1 + digit2;
        res[maxlen - i] = (sum % base < 10) ? (sum % base) + '0' : (sum % base) - 10 + 'A';
        carry = sum / base;
    }

    res[0] = (carry > 0) ? (carry + '0') : '0';
    char* final_res = check_0(res);
    if (!final_res) {
        free(firstnum_fixed);
        free(secondnum_fixed);
        free(res);
        return MEMORY_ALLOCATION_ERROR;
    }

    *result = final_res;
    free(firstnum_fixed);
    free(secondnum_fixed);
    return SUCCESS;
}

StatusCode amount(int base, int count, char** result, ...) {
    if (base < 2 || base > 36) {
        return INVALID_BASE_ERROR;
    }

    va_list vl;
    va_start(vl, result);

    for (int i = 0; i < count; ++i) {
        if (va_arg(vl, char*) == NULL) {
            va_end(vl);
            return INVALID_ARGUMENT_COUNT_ERROR;
        }
    }
    va_end(vl);

    char* total_sum = (char*)malloc(sizeof(char) * 2);
    if (!total_sum) return MEMORY_ALLOCATION_ERROR;
    strcpy(total_sum, "0");

    va_start(vl, result);
    for (int i = 0; i < count; ++i) {
        char* tmp_value = va_arg(vl, char*);
        if (!(isdigit(*tmp_value) || isalpha(*tmp_value)) ||
            (isdigit(*tmp_value) && (*tmp_value - '0' >= base)) ||
            (isalpha(*tmp_value) && (toupper(*tmp_value) - 'A' + 10 >= base))) {
            free(total_sum);
            va_end(vl);
            return INVALID_NUMBER_ERROR;
        }

        char* temp = NULL;
        StatusCode status = sum(total_sum, tmp_value, base, &temp);
        if (status != SUCCESS) {
            free(total_sum);
            va_end(vl);
            return status;
        }
        free(total_sum);
        total_sum = temp;
    }
    va_end(vl);

    *result = total_sum;
    return SUCCESS;
}


int main() {
    char num1[] = "12";
    char num2[] = "33";
    char* result;

    StatusCode status = amount(16, 3, &result, num1, num2);
    switch (status) {
        case SUCCESS:
            printf("Sum: %s\n", result);
            free(result);
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation failed.\n");
            break;
        case INVALID_NUMBER_ERROR:
            printf("Invalid number provided.\n");
            break;
        case INVALID_BASE_ERROR:
            printf("Invalid base provided.\n");
            break;
        case INVALID_ARGUMENT_COUNT_ERROR:
            printf("Invalid argument count provided.\n");
            break;
        default:
            printf("Unknown error occurred.\n");
            break;
    }

    return 0;
}

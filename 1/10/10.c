#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TO_UPPER(x) (x - (x >= 'a' && x <= 'z') * ('a' - 'A'))
#define IS_DIG(x) ((x >= '0') && (x <= '9'))
#define IS_LET(x) ((TO_UPPER(x) >= 'A') && (TO_UPPER(x) <= 'Z'))
#define TO_DIG(x) (((IS_DIG(x)) * (x - '0') + (IS_LET(x)) * (TO_UPPER(x) - 'A' + 10)))
#define TO_CHAR(x) (((x >= 0) && (x <= 9)) * (x + '0') + ((x >= 10) && (x <= 35)) * (x + 'A' - 10))

int my_pow(int n, int k) {
    if (k == 0)
        return 1;
    return n * my_pow(n, k - 1);
}

int in_10_base(int base, char* number) {
    int ans = 0;
    int len = strlen(number);

    for (int i = 0; i < len; i++) {
        int digit = TO_DIG(number[i]);
        if (digit == -1 || digit >= base) {
            printf("Error: digit %c exceeds base %d\n", number[i], base);
            return -1;
        }
        ans += digit * my_pow(base, len - 1 - i);
    }
    return ans;
}

void to_base(int base, int number) {
    if (number == 0) {
        printf("0 in base %d = 0\n", base);
        return;
    }

    char based_number[BUFSIZ];
    int len = 0;
    
    while (number > 0) {
        based_number[len++] = TO_CHAR(number % base);
        number /= base;
    }

    // Реверс строки
    for (int i = 0; i < len / 2; i++) {
        char temp = based_number[i];
        based_number[i] = based_number[len - 1 - i];
        based_number[len - 1 - i] = temp;
    }
    based_number[len] = '\0';

    printf("%d in base %d = %s\n", number, base, based_number);
}

int main() {
    int base;
    char str[BUFSIZ];
    int max = 0;
    char stop[] = "Stop";

    printf("Enter base (2-36): ");
    scanf("%d", &base);

    if (base < 2 || base > 36) {
        printf("Base must be in range [2..36]\n");
        return 1;
    }

    printf("Enter numbers (type 'Stop' to finish):\n");
    while (1) {
        scanf("%s", str);
        if (strcmp(str, stop) == 0)
            break;

        int tmp = in_10_base(base, str);
        if (tmp < 0) {
            printf("Invalid number: %s\n", str);
            continue;
        }

        if (abs(tmp) > abs(max)) {
            max = tmp;
        }
    }

    printf("Max value in base 10 = %d\n", max);

    for (int tmp = 9; tmp <= 36; tmp += 9) {
        to_base(tmp, abs(max));
    }

    return 0;
}

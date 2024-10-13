    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    int my_atoi(char* str) {
        int result = 0;
        int sign = 1;
        int i = 0;

        if (str[0] == '-') {
            sign = -1;
            i++;
        }

        for (; str[i] != '\0'; i++) {
            if (str[i] < '0' || str[i] > '9')
                return 0;
            result = result * 10 + (str[i] - '0');
        }

        return result * sign;
    }

    void flag_h(int n) {
        for (int i = 1; i <= 100; i++) {
            if (n > 100) {
                printf("No such numbers\n");
                return;
            } else {
                if (i % n == 0)
                    printf("%d ", i);
            }
        }
        printf("\n");
    }

    int flag_p(int n) {
        if (n <= 1) {
            printf("Number is neither prime nor composite\n");
            return 0;
        }
        for (int i = 2; i <= sqrt(abs(n)); i++) {
            if (n % i == 0) {
                printf("Number is composite\n");
                return 0;
            }
        }
        printf("Number is prime\n");
        return 1;
    }

    void flag_s(int n) {
        if (n == 0) {
            printf("0\n");
            return;
        }
        unsigned int num = (unsigned int) abs(n);
        while (num != 0) {
            printf("%X ", num % 16);
            num = num / 16;
        }
        printf("\n");
    }

    void my_pow(int n, int pow) {
        int a = 1;
        printf("%d ", a);
        while (pow--) {
            a = a * n;
            printf("%d ", a);
        }
        printf("\n");
    }

    void flag_e(int n) {
        if (n > 10) {
            printf("Number is greater than 10\n");
            return;
        }
        for (int i = 1; i <= 10; i++)
            my_pow(i, n);
    }

    void flag_a(int n) {
        printf("%d\n", (1 + n) * n / 2);
    }

    void flag_f(int n) {
        if (n < 0) {
            printf("Factorial is not defined for negative numbers\n");
            return;
        }
        int a = 1;
        while (n > 1) {
            a = a * n;
            n--;
        }
        printf("%d\n", a);
    }

    int flag(int n, char op) {
        switch (op) {
        case 'h':
            flag_h(n);
            return 0;
        case 'p':
            flag_p(n);
            return 0;
        case 's':
            flag_s(n);
            return 0;
        case 'e':
            flag_e(n);
            return 0;
        case 'a':
            flag_a(n);
            return 0;
        case 'f':
            flag_f(n);
            return 0;
        default:
            printf("Flag not found\n");
            return 1;
        }
    }

    int main(int argc, char* argv[]) {
        int n;
        char op = '\0';
        int flag_index = -1;

        if (argc == 3) {
            for (int i = 1; i <= 2; i++) {
                if (argv[i][0] == '-' || argv[i][0] == '/') {
                    op = argv[i][1];
                    flag_index = i;
                    break;
                }
            }

            if (flag_index == -1) {
                printf("Flag not found\n");
                return 1;
            }

            if (flag_index == 1)
                n = my_atoi(argv[2]);
            else
                n = my_atoi(argv[1]);

            if (n == 0 && argv[flag_index == 1 ? 2 : 1][0] != '0') {
                printf("Invalid number\n");
                return 1;
            }

            if (flag(n, op) != 0) {
                return 1;
            }
        } else {
            printf("Wrong number of arguments\n");
            return 1;
        }

        return 0;
    }

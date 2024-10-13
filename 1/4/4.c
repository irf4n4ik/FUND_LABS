#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum kOpts {
    OPT_D,
    OPT_I,
    OPT_S,
    OPT_A,
    OPT_INVALID
} kOpts;

int GetOpts(int argc, char** argv, kOpts *option, char **file_path, char **output_path) {
    if (argc < 3) {
        printf("Error: Not enough arguments provided.\n");
        return 1;
    }

    const char* flag_str = argv[1];
    int needs_output_path = 0;

    if (flag_str[0] == '-' || flag_str[0] == '/') {
        int flag_index = 1;
        if (flag_str[1] == 'n') {
            needs_output_path = 1;
            flag_index++;
        }

        switch (flag_str[flag_index]) {
            case 'd':
                *option = OPT_D;
                break;
            case 'i':
                *option = OPT_I;
                break;
            case 's':
                *option = OPT_S;
                break;
            case 'a':
                *option = OPT_A;
                break;
            default:
                printf("Error: Invalid flag '%s'.\n", flag_str);
                *option = OPT_INVALID;
                return 1;
        }
    } else {
        printf("Error: Invalid flag format. Flags must start with '-' or '/'.\n");
        *option = OPT_INVALID;
        return 1;
    }

    *file_path = argv[2];

    if (*file_path == NULL || strlen(*file_path) == 0) {
        printf("Error: Invalid input file path.\n");
        return 1;
    }

    if (needs_output_path) {
        if (argc == 4) {
            *output_path = argv[3];
            if (*output_path == NULL || strlen(*output_path) == 0) {
                printf("Error: Invalid output file path.\n");
                return 1;
            }
        } else {
            printf("Error: Missing output file path for 'n' flag.\n");
            return 1;
        }
    } else {
        if (argc > 3) {
            printf("Error: Output file path should not be provided unless using 'n' flag.\n");
            return 1;
        }
        *output_path = NULL;
    }

    return 0;
}

void HandlerOptD(FILE* file, const char* output_path) {
    FILE* out_file = fopen(output_path, "w");
    if (!out_file) {
        printf("Error: Unable to open output file '%s' for writing.\n", output_path);
        return;
    }

    char a;
    while ((a = fgetc(file)) != EOF) {
        if (!isdigit(a)) {
            fputc(a, out_file);
        }
    }
    fclose(out_file);
}

void HandlerOptI(FILE* file, const char* output_path) {
    FILE* out_file = fopen(output_path, "w");
    if (!out_file) {
        printf("Error: Unable to open output file '%s' for writing.\n", output_path);
        return;
    }

    char a;
    int count = 0;
    int str = 1;
    while ((a = fgetc(file)) != EOF) {
        if (isalpha(a)) count++;
        if (a == '\n') {
            fprintf(out_file, "%d string has %d letters\n", str, count);
            str++;
            count = 0;
        }
    }
    if (count > 0) {
        fprintf(out_file, "%d string has %d letters", str, count);
    }
    fclose(out_file);
}

void HandlerOptS(FILE* file, const char* output_path) {
    FILE* out_file = fopen(output_path, "w");
    if (!out_file) {
        printf("Error: Unable to open output file '%s' for writing.\n", output_path);
        return;
    }

    int count = 0;
    int str = 1;
    int a;

    while ((a = fgetc(file)) != EOF) {
        if (!(isalpha(a) || isdigit(a) || isspace(a))) {
            count++;
        }

        if (a == '\n') {
            fprintf(out_file, "%d string has %d special symbols\n", str, count);
            str++;
            count = 0;
        }
    }

    if (count > 0) {
        fprintf(out_file, "%d string has %d special symbols\n", str, count);
    }

    fclose(out_file);
}

void HandlerOptA(FILE* file, const char* output_path) {
    FILE* out_file = fopen(output_path, "w");
    if (!out_file) {
        printf("Error: Unable to open output file '%s' for writing.\n", output_path);
        return;
    }

    char a;
    while ((a = fgetc(file)) != EOF) {
        if (!isdigit(a)) {
            fprintf(out_file, "%x", (int)a);
        } else {
            fputc(a, out_file);
        }
        if (a == '\n') {
            fputc('\n', out_file);
        }
    }
    fclose(out_file);
}

int main(int argc, char** argv) {
    kOpts opt;
    char* input_path;
    char* output_path;

    if (GetOpts(argc, argv, &opt, &input_path, &output_path)) {
        return 1;
    }

    FILE* file = fopen(input_path, "r");
    if (!file) {
        printf("Error: Unable to open input file '%s' for reading.\n", input_path);
        return 1;
    }

    if (!output_path) {
        output_path = (char*)malloc(strlen("out_") + strlen(input_path) + 1);
        if (!output_path) {
            printf("Error: Memory allocation failed for output file path.\n");
            fclose(file);
            return 1;
        }
        strcpy(output_path, "out_");
        strcat(output_path, input_path);
    }

    if (strcmp(input_path, output_path) == 0) {
        printf("Error: Input and output file paths must be different.\n");
        fclose(file);
        if (output_path && argv[3] != output_path) {
            free(output_path);
        }
        return 1;
    }

    void (*handlers[])(FILE*, const char*) = {
        HandlerOptD,
        HandlerOptI,
        HandlerOptS,
        HandlerOptA
    };

    if (opt != OPT_INVALID) {
        handlers[opt](file, output_path);
    } else {
        printf("Invalid flag.\n");
    }

    fclose(file);
    if (output_path && argv[3] != output_path) {
        free(output_path);
    }

    return 0;
}

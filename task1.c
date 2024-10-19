#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



int strings_equal(const char* a, const char* b) {
    return !strcmp(a, b);
}

size_t string_length(const char* str) {
    size_t length = 0;
    while (*str++) {
        length++;
    }
    return length;
}

typedef int (*operation)(int, char**);

int get_flag_index(const char* arg, const char** flag_list, int flag_count) {
    for (int i = 0; i < flag_count; i++) {
        if (strings_equal(arg, flag_list[i])) {
            return i;
        }
    }
    return -1;
}
char* reverse_string(const char* str) {
    int length = string_length(str);
    
    char* reversed_val = (char*)malloc((length + 1) * sizeof(char));
    if (reversed_val == NULL) {
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        reversed_val[i] = str[length - i - 1];
    }
    reversed_val[length] = '\0';
    return reversed_val;
}



char* sort_characters(const char* str) {
    int length = string_length(str);

    char* digits = (char*)malloc((length + 1) * sizeof(char));
    char* letters = (char*)malloc((length + 1) * sizeof(char));
    char* symbols = (char*)malloc((length + 1) * sizeof(char));
    if (digits == NULL || letters == NULL || symbols == NULL) {
        return NULL;
    }

    char *d_ptr = digits;
    char *l_ptr = letters;
    char *s_ptr = symbols;

    while (*str) {
        if (*str >= '0' && *str <= '9') {
            *d_ptr++ = *str;
        } else if ((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z')) {
            *l_ptr++ = *str;
        } else {
            *s_ptr++ = *str;
        }
        str++;
    }

    *d_ptr = '\0';
    *l_ptr = '\0';
    *s_ptr = '\0';

    char* sorted_str = (char*)malloc((length + 1) * sizeof(char));
    if (sorted_str == NULL) {
        free(digits);
        free(letters);
        free(symbols);
        return NULL;
    }

    char *result_ptr = sorted_str;
    d_ptr = digits;
    l_ptr = letters;
    s_ptr = symbols;

    while (*d_ptr) {
        *result_ptr++ = *d_ptr++;
    }

    while (*l_ptr) {
        *result_ptr++ = *l_ptr++;
    }

    while (*s_ptr) {
        *result_ptr++ = *s_ptr++;
    }

    *result_ptr = '\0';

    free(digits);
    free(letters);
    free(symbols);

    return sorted_str;
}

char* concatenate_random(int argc, char** argv) {
    int total_len = 0;

    for (int i = 3; i < argc; i++) {
        total_len += string_length(argv[i]);
    }

    unsigned long random_seed = strtoul(argv[2], NULL, 0);
    srand(random_seed);

    char* concatenated = (char*)malloc((total_len + 1) * sizeof(char));
    if (concatenated == NULL) {
        return NULL;
    }

    char* result_ptr = concatenated;
    bool used_flags[argc];
    for (int i = 0; i < argc; i++) {
        used_flags[i] = false;
    }

    int remaining_count = argc - 3;

    while (remaining_count > 0) {
        int index = 3 + rand() % (argc - 3);

        if (!used_flags[index]) {
            used_flags[index] = true;
            remaining_count--;

            char* temp_str = argv[index];
            while (*temp_str) {
                *result_ptr++ = *temp_str++;
            }
        }
    }

    *result_ptr = '\0';

    return concatenated;
}

char* alternate_uppercase(const char* str) {
    int length = string_length(str);
    
    char* modified_str = (char*)malloc((length + 1) * sizeof(char));
    if (modified_str == NULL) {
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        if (str[i] >= 'a' && str[i] <= 'z' && (i % 2 == 0)) {
            modified_str[i] = str[i] - 32;
        } else {
            modified_str[i] = str[i];
        }
    }
    modified_str[length] = '\0';
    return modified_str;
}

int for_flag_l(int argc, char** argv) {
    int result = string_length(argv[2]);
    printf("String length: %d\n", result);
    return 0;
}

int for_flag_r(int argc, char** argv) {
    char* result = reverse_string(argv[2]);
    if (result == NULL) {
        printf("Не смог выделить память\n");
        return -1;
    }
    printf("Reversed: %s\n", result);
    free(result);
    return 0;
}

int for_flag_u(int argc, char** argv) {
    char* result = alternate_uppercase(argv[2]);
    if (result == NULL) {
        printf("Не смог выделить память\n");
        return -1;
    }
    printf("Modified case: %s\n", result);
    free(result);
    return 0;
}

int for_flag_n(int argc, char** argv) {
    char* result = sort_characters(argv[2]);
    if (result == NULL) {
        printf("Не смог выделить память\n");
        return -1;
    }
    printf("Sorted characters: %s\n", result);
    free(result);
    return 0;
}

int for_flag_c(int argc, char** argv) {
    char* result = concatenate_random(argc, argv);
    if (result == NULL) {
        printf("Не смог выделить память\n");
        return -1;
    }
    printf("Random concatenation: %s\n", result);
    free(result);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <flag> <input>\n", argv[0]);
        return -1;
    }

    const char* flags_from_user_input[] = {"-l", "-r", "-u", "-n", "-c"};
    operation functions[] = {&for_flag_l, &for_flag_r, &for_flag_u, &for_flag_n, &for_flag_c};
    for (int i = 1; i < argc; i++) {
        int flag_index = get_flag_index(argv[i], flags_from_user_input, sizeof(flags_from_user_input) / sizeof (const char*));
        if (flag_index == -1) {
            printf("Unknown option: %s\n", argv[i]);
            return -1;
        } else {
            operation chosen_operation = functions[flag_index];
            int result = chosen_operation(argc, argv);
            if (result == -1) {
                printf("An error occurred during execution\n");
                return -1;
            }
            return 0;
        }
    }
    return 0;
}

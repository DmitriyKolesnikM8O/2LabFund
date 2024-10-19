#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>


int is_valid_number(const char *str, int base) {
    if (str == NULL || *str == '\0') return 0;
    if (*str == '-') return 0;
    
    while (*str) {
        if (!isalnum(*str)) 
        return 0; 
        int digit_value = isdigit(*str) ? *str - '0' : tolower(*str) - 'a' + 10;
        if (digit_value >= base) 
        return 0;
        str++;
    }
    return 1;
}


int is_kaprekar(int number, int base) {
    if (number == 0) return 0;

    long square = (long)number * number;
    char square_str[100];
    sprintf(square_str, "%ld", square); 

    int len = strlen(square_str); 
    for (int i = 1; i < len; i++) {
        char left_part[100], right_part[100];

        strncpy(left_part, square_str, i);
        left_part[i] = '\0';
        strcpy(right_part, square_str + i);

        int left = (int)strtol(left_part, NULL, base);
        int right = (int)strtol(right_part, NULL, base);

        if (left + right == number && right > 0) {
            return 1;
        }
    }
    return 0;
}


void check_kaprekar_numbers(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    printf("Числа Капрекара в системе с основанием %d:\n", base);
    for (int i = 0; i < count; i++) {
        char *num_str = va_arg(args, char*);
        if (!is_valid_number(num_str, base)) {
            printf("%s не является корректным числом в системе с основанием %d.\n", num_str, base);
            continue;
        }

        int number = (int)strtol(num_str, NULL, base);
        if (is_kaprekar(number, base)) {
            printf("%s является числом Капрекара.\n", num_str);
        } else {
            printf("%s не является числом Капрекара.\n", num_str);
        }
    }

    va_end(args);
}

int main() {

    printf("Пример 1:\n");
    check_kaprekar_numbers(10, 4, "9", "45", "4", "100");

    printf("\nПример 2:\n");
    check_kaprekar_numbers(10, 3, "297", "703", "1000");

    printf("\nПример 3:\n");
    check_kaprekar_numbers(2, 2, "1", "10"); // Примеры в двоичной системе

    printf("\nПример 4:\n");
    check_kaprekar_numbers(16, 3, "4", "1A", "3E"); // Примеры в шестнадцатеричной системе

    return 0;
}

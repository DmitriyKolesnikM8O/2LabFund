#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

int char_to_value(char c) {
    if (isdigit(c)) {
        return c - '0';
    } else {
        return toupper(c) - 'A' + 10;
    }
}

char value_to_char(int value) {
    if (value < 10) {
        return value + '0';
    } else {
        return value - 10 + 'A';
    }
}

char* column_addition(const char* num1, const char* num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int max_len = len1 > len2 ? len1 : len2;

    char* result = (char*)malloc(max_len + 2);
    result[max_len + 1] = '\0';

    int carry = 0;
    int result_index = max_len;

    for (int i = 0; i < max_len; i++) {
        int digit1 = (len1 - i - 1 >= 0) ? char_to_value(num1[len1 - i - 1]) : 0;
        int digit2 = (len2 - i - 1 >= 0) ? char_to_value(num2[len2 - i - 1]) : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        result[result_index--] = value_to_char(sum % base);
    }

    if (carry > 0) {
        result[result_index--] = value_to_char(carry);
    }

    return &result[result_index + 1];
}

char* sum_in_base(int base, int count, ...) {
    va_list args;
    va_start(args, count);
    
    char* total_sum = strdup(va_arg(args, char*));

    for (int i = 1; i < count; i++) {
        char* new_sum = column_addition(total_sum, va_arg(args, char*), base);
        // free(total_sum);
        total_sum = new_sum;
    }

    va_end(args);
    
    // Удаляем ведущие нули
    while (*total_sum == '0' && *(total_sum + 1) != '\0') {
        total_sum++;
    }

    return strdup(total_sum);
}

int is_valid_number(const char* num, int base) {
    for (int i = 0; num[i] != '\0'; i++) {
        int value = char_to_value(num[i]);
        if (value < 0 || value >= base) {
            return 0;
        }
    }
    return 1;
}

int is_valid_integer_for_input(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {

    char* sum_result;

    // Пример 1: Сумма двух чисел
    sum_result = sum_in_base(30, 2, "1000000000000000000", "200000000000000000000000000000000");
    printf("Сумма чисел 1A и 2B в системе счисления с основанием %d: %s\n", 30, sum_result);
    free(sum_result);

    // Пример 2: Сумма трех чисел
    sum_result = sum_in_base(30, 3, "1C", "2D", "3E");
    printf("Сумма чисел 1C, 2D и 3E в системе счисления с основанием %d: %s\n", 30, sum_result);
    free(sum_result);

    // Пример 3: Сумма четырех чисел
    sum_result = sum_in_base(30, 4, "FF", "1", "A", "B");
    printf("Сумма чисел FF, 1, A и B в системе счисления с основанием %d: %s\n", 30, sum_result);
    free(sum_result);

    return 0;
}

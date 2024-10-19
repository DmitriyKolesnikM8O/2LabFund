#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

double dichotomy(double (*func)(double), double a, double b, double epsilon) {
    if (a > b) {
        double temp = a;
        a = b;
        b = temp;
    }
    if (func(a) * func(b) >= 0) {
        return -1; 
    }

    double mid; 
    while ((b - a) / 2 > epsilon) {
        mid = (a + b) / 2;
        if (fabs(func(mid)) < epsilon)
            return mid;
        else if (func(mid) * func(a) < 0) // корень от а до c
            b = mid;
        else // корень от с до b
            a = mid;
    }

    return (a + b) / 2; 
}

// x^3 - x - 2 [1, 2]
double example_1(double x) {
    return x * x * x - x - 2;
}

// sin(x) - 0.5 [0, 2]
double example_2(double x) {
    return sin(x) - 0.5;
}

// x^2 - 4 [0, 3]
double example_3(double x) {
    return x * x - 4;
}

//для ввода
int is_number(const char* str) {
    
    int i = 0;
    int has_decimal_point = 0;

    
    while (isspace(str[i])) i++;

    
    if (str[i] == '-' || str[i] == '+') i++;

    while (str[i] != '\0') {
        if (isdigit(str[i])) {
            i++;
        } else if (str[i] == '.' && !has_decimal_point) {
            has_decimal_point = 1;
            i++;
        } else if (isspace(str[i])) {
            i++;
        } else {
            return 0; 
        }
    }

    return 1;
}

double input_user(const char* prompt) {
    char buffer[4096];

    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Ошибка ввода! Пожалуйста, попробуйте снова.\n");
            continue;
        }

        
        buffer[strcspn(buffer, "\n")] = 0;

        
        if (!is_number(buffer)) {
            printf("Ошибка ввода! Пожалуйста, введите число.\n");
        } else {
            double value;
            sscanf(buffer, "%lf", &value);
            return value; 
        }
    }
}

double input_epsilon(const char* prompt) {
    double value;
    while (1) {
        value = input_user(prompt);
        if (value <= 0) {
            printf("Точность должна быть положительным числом!\n");
        } else {
            return value;
        }
    }
}

int main() {
    double a, b, epsilon;

    a = input_user("Введите левую границу интервала: ");
    b = input_user("Введите правую границу интервала: ");
    epsilon = input_epsilon("Введите точность (epsilon): ");

    double root_1 = dichotomy(example_1, a, b, epsilon); 

    if (root_1 != -1) {
        printf("Приближённый корень уравнения x^3 - x - 2: %.5f\n", root_1);
    } else {
        printf("Не удалось вычислить корень уравнения x^3 - x - 2.\n");
    }

    
    double root_2 = dichotomy(example_2, a, b, epsilon);
    if (root_2 != -1) {
        printf("Приближённый корень уравнения sin(x) - 0.5: %.5f\n", root_2);
    } else {
        printf("Не удалось вычислить корень уравнения sin(x) - 0.5.\n");
    }

    double root_3 = dichotomy(example_3, a, b, epsilon);
    if (root_3 != -1) {
        printf("Приближённый корень уравнения x^2 - 4: %.5f\n", root_3);
    } else {
        printf("Не удалось вычислить корень уравнения x^2 - 4.\n");
    }

    return 0;
}

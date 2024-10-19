#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Здесь мы возводим в степень
int my_pow(double *res, double x, int n) {
    *res = 1; 
    
    if (n < 0) {
        x = 1 / x;
        n *= -1;  
    }
    
    
    for (int i = 0; i < n; i++) {
        *res *= x;
    }
    
    return 0;
}

//тут считаем биноминальные коэффициентики наши
int Newton_koeffff(double *result, int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        printf("Недопустимые значения n и k\n");
        return -1;
    }
    *result = 1;
    for (int i = n - k + 1; i <= n; i++) {
        *result *= i;
    }
    for (int i = 2; i <= k; i++) {
        *result /= i;
    }
    return 0;
}


//здесь уже считаем само разложение
int decomposition(double a, int n, double f, double *res) {
    for (int k = 0; k <= n; k++) {
        double bin;

        if (Newton_koeffff(&bin, n, k) != 0) {
            printf("Ошибка при вычислении коэффициентов\n");
            return -1;
        }

        double ap;
        if (my_pow(&ap, a, k) != 0) {
            printf("Ошибка в функции pow_custom\n");
            return -1;
        }

        res[n - k] += bin * ap * f;
    }
    return 0;
}


int convert_to_massive(double a, double **res, int n, ...) {
    double *fs = malloc((n + 1) * sizeof(double));
    if (!fs) {
        printf("Не удалось выделить память\n");
        return -1;
    }

    
    va_list valist;
    va_start(valist, n);

    
    for (int i = 0; i <= n; i++) {
        fs[i] = va_arg(valist, double);
    }
    va_end(valist);

    
    double *koef = malloc((n + 1) * sizeof(double));
    if (!koef) {
        free(fs);
        printf("Не удалось выделить память\n");
        return -1;
    }

    
    for (int i = 0; i <= n; i++) {
        koef[i] = 0.0; 
    }

    
    for (int i = n; i >= 0; i--) {
        int result = decomposition(a, i, fs[i], koef);
        if (result != 0) {
            free(fs);
            free(koef);
            printf("Ошибка в функции decomposition\n");
            return -1;
        }
    }

    
    free(fs);
    *res = koef;
    return 0;
}

int main(void) {
    double *result;

    // // Первая конверсия
    // printf("Результаты конвертации для n = 5:\n");
    // if (convert_to_massive(0.9, &result, 5, 2.4, 0.5, -0.2, 12.4, -8.2, 12.1) != 0) {
    //     printf("Ошибка при конвертации для n = %d\n", 5);
    //     return -1;
    // }
    
    // printf("Результаты:\n");
    // for (int i = 0; i <= 5; i++) {
    //     printf("g[%d] = %f\n", i, result[i]); // Явное указание индекса
    // }
    // printf("\n");
    // free(result);

    // Вторая конверсия
    printf("Для примера x^4 - 3x^2 + x - 2:\n");
    if (convert_to_massive(3, &result, 4, -2.0, 1.0, -3.0, 0.0, 1.0) != 0) {
        printf("Ошибка при конвертации для n = %d\n", 4);
        return -1;
    }
    
    printf("Результаты:\n");
    for (int i = 0; i <= 4; i++) {
        printf("g[%d] = %f\n", i, result[i]); // Явное указание индекса
    }
    printf("\n");
    free(result);

    return 0;
}

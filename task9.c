#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int check_final_representation(bool *result, int ss, double x, double e) {
    int cap = 128;
    double *remains = malloc(cap * sizeof(double));
    if (!remains) {
        printf("Ошибка при выделении памяти\n");
        return -1;
    }
    
    *result = false;
    
    int i = 0;
    while (1) {
        x *= ss;
        x -= (int)x;
        if ((int)ceil(x) == 0) {
            *result = true;
            break;
        }
        bool end = false;
        for (int j = 0; j < i; j++) {
            if (fabs(x - remains[j]) < e) {
                end = true;
                break;
            }
        }
        if (end)
            break;

        if (i >= cap) {
            int new_cap = cap * 2;
            double *remains_realloc = realloc(remains, new_cap * sizeof(double));
            if (!remains_realloc) {
                free(remains);
                printf("Ошибка при выделении памяти\n");
                return -1;
            }
            remains = remains_realloc;
            cap = new_cap;
        }
        remains[i] = x;
        i++;
    }
    free(remains);
    return 0;
}

int main_func_for_representaion(int ss, ...) {
    if (ss < 2) {
        printf("Некорректно введена система счисления\n");
        return -1;
    }

    va_list valist;
    va_start(valist, ss);

    while (1) { 
        double x = va_arg(valist, double);
        if (x <= 0 || x >= 1) {
            printf("Некорректно введено число: %f\n", x);
            break; 
        }

        bool fin = false; 
        int res = check_final_representation(&fin, ss, x, 0.1); 

        if (res != 0) { 
            break;
        }

        if (!fin) { 
            printf("%f не имеет конечного представления\n", x);
            continue; 
        }
            printf("%f имеет конечное представление\n", x);
    }

    va_end(valist); 
    return 0; 
}

int main(void) {
    printf("Для 2-сс:\n");
    main_func_for_representaion(2, 0.1, 0.25, 0.5, 0.9, 0.33, 1.0);
    printf("Для 10-сс:\n");
    main_func_for_representaion(10, 0.1, 0.25, 0.5, 0.9, 0.33, 1.0);
}

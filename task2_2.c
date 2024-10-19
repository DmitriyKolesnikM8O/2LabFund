#include <stdio.h>


// double fast_pow(double base, int exp) {
//     if (exp == 0) {
//         return 1.0;
//     }
    
//     if (exp < 0) {
//         return 1.0 / fast_pow(base, -exp);
//     }

//     double half_pow = fast_pow(base, exp / 2);
    
//     if (exp % 2 == 0) {
//         return half_pow * half_pow;
//     } else {
//         return base * half_pow * half_pow;
//     }
// }


double fast_pow_iterative(double base, int exp) {
    double result = 1.0;
    int exponent = exp;

    if (exp < 0) {
        base = 1.0 / base;
        exponent = -exp;
    }

    while (exponent > 0) {
        if (exponent % 2 != 0) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }

    return result;
}

int main() {
    double res;

    // Пример 1: возведение 2.0 в степень 10
    res = fast_pow_iterative(2.0, 10);
    printf("%.2lf в степени %d равно %.6lf\n", 2.0, 10, res);

    // Пример 2: возведение 2.0 в степень -10
    res = fast_pow_iterative(2.0, -10);
    printf("%.2lf в степени %d равно %.6lf\n", 2.0, -10, res);

    // Пример 3: возведение 2.0 в степень 0
    res = fast_pow_iterative(2.0, 0);
    printf("%.2lf в степени %d равно %.6lf\n", 2.0, 0, res);

    // Пример 4: возведение 2.0 в степень 1
    res = fast_pow_iterative(2.0, 1);
    printf("%.2lf в степени %d равно %.6lf\n", 2.0, 1, res);

    return 0;
}


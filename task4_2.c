#include <stdio.h>
#include <stdarg.h>
#include <math.h>

double polynomial_value(double x, int degree, ...) {
    va_list args;
    va_start(args, degree);

    double result = 0;
    for (int i = 0; i <= degree; i++) {
        
        double coeff = va_arg(args, double);
        
        result += coeff * pow(x, degree - i);
    }

    va_end(args);
    return result;
}

/*
Examples: 
3x^2 + 2x + 1, x = 2 --> 17.00
4x^3 - 5x^2 + 3x - 2, x = 1.5 --> 4.75
*/

int main() {
    double result = polynomial_value(2.0, 2.0, 3.0, 2.0, 1.0);
    printf("Значение многочлена 3x^2 + 2x + 1, x = 2: %.2lf\n", result);

    result = polynomial_value(1.5, 3.0, 4.0, -5.0, 3.0, -2.0);
    printf("Значение многочлена 4x^3 - 5x^2 + 3x - 2, x = 1.5: %.2lf\n", result);
    return 0;
}

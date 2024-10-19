#include <stdio.h>
#include <stdarg.h>
#include <math.h>

double geometric_mean(int count, ...) {
    va_list args;
    va_start(args, count);

    double product = 1.0;
    for (int i = 0; i < count; i++) {
        double num = va_arg(args, double);
        product *= num;
    }
    
    va_end(args);

    return pow(product, 1.0 / count);
}

int main() {
    double result = geometric_mean(3, 2.0, 8.0, 4.0);
    printf("Geometric Mean (2.0, 8.0, 4.0): %f\n", result);
    result = geometric_mean(4, 1.0, 2.0, 3.0, 4.0);
    printf("Geometric Mean (1.0, 2.0, 3.0, 4.0): %f\n", result);
    
    return 0;
}

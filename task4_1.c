#include <stdio.h>
#include <stdarg.h>

typedef struct {
    double x;
    double y;
} Point;

double cross_product(Point p1, Point p2, Point p3) {
    return (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
}

int is_convex(int num_points, ...) {
    if (num_points < 3) {
        printf("Многоугольник должен иметь минимум 3 вершины.\n");
        return 0;
    }

    va_list args;
    va_start(args, num_points);
    

    Point points[num_points];
    

    for (int i = 0; i < num_points; i++) {
        points[i].x = va_arg(args, double);
        points[i].y = va_arg(args, double);
    }
    
    va_end(args);

    int sign = 0;
    for (int i = 0; i < num_points; i++) {
        double cp = cross_product(points[i], points[(i + 1) % num_points], 
                                   points[(i + 2) % num_points]);
        if (cp != 0) {
            if (sign == 0) {
                sign = (cp > 0) ? 1 : -1;
            } else if ((cp > 0 && sign < 0) || (cp < 0 && sign > 0)) {
                return 0;  
            }
        }
    }
    
    return 1;
}

int main() {
    // Пример 1: Треугольник
    printf("Пример 1:\n");
    printf("Точки: 0.0, 0.0,  1.0, 1.0,  2.0, 0.0\n");
    if (is_convex(3, 0.0, 0.0,  1.0, 1.0,  2.0, 0.0)) {
        printf("Многоугольник выпуклый.\n");
    } else {
        printf("Многоугольник невыпуклый.\n");
    }

    // Пример 2: Четырехугольник (выпуклый)
    printf("\nПример 2:\n");
    printf("Точки: 0.0, 0.0,  2.0, 2.0,  2.0, 0.0,  0.0, -2.0\n");
    if (is_convex(4, 0.0, 0.0,  2.0, 2.0,  2.0, 0.0,  0.0, -2.0)) {
        printf("Многоугольник выпуклый.\n");
    } else {
        printf("Многоугольник невыпуклый.\n");
    }

    // Пример 3: Четырехугольник (невыпуклый)
    printf("\nПример 3:\n");
    printf("Точки: 0.0, 0.0,  2.0, 1.0,  1.0, -1.0,  0.0, -2.0\n");
    if (is_convex(4, 0.0, 0.0,  2.0, 1.0,  1.0, -1.0,  0.0, -2.0)) {
        printf("Многоугольник выпуклый.\n");
    } else {
        printf("Многоугольник невыпуклый.\n");
    }

    return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <stdarg.h>

// typedef struct {
//     double x;
//     double y;
// } Point;

// double cross_product(Point p1, Point p2, Point p3) {
//     return (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
// }

// int is_convex(int num_points, Point* points) {
//     if (num_points < 3) {
//         printf("Многоугольник должен иметь минимум 3 вершины.\n");
//         return 0;
//     }
    
//     int sign = 0;
//     for (int i = 0; i < num_points; i++) {
//         double cp = cross_product(points[i], points[(i + 1) % num_points], 
//                                    points[(i + 2) % num_points]);
//         if (cp != 0) {
//             if (sign == 0) {
//                 sign = (cp > 0) ? 1 : -1;
//             } else if ((cp > 0 && sign < 0) || (cp < 0 && sign > 0)) {
//                 return 0;  
//             }
//         }
//     }
    
//     return 1;
// }

// int main() {
//     int num_points;
//     printf("Введите количество вершин многоугольника: ");
//     while (scanf("%d", &num_points) != 1 || num_points < 3) {
//         printf("Неверный ввод. Введите целое число больше или равное 3: ");
//         while (getchar() != '\n');  // Очистка ввода
//     }

    
//     Point* points = (Point*)malloc(num_points * sizeof(Point));
//     if (points == NULL) {
//         printf("Ошибка выделения памяти.\n");
//         return 1;
//     }

//     double x, y;
//     printf("Введите координаты вершин (x и y через пробел):\n");
//     for (int i = 0; i < num_points; i++) {
//         printf("Вершина %d: ", i + 1);
//         while (scanf("%lf %lf", &x, &y) != 2) {
//             printf("Неверный ввод. Введите координаты x и y через пробел: ");
//             while (getchar() != '\n');
//         }
//         points[i].x = x;
//         points[i].y = y;
//     }

//     if (is_convex(num_points, points)) {
//         printf("Многоугольник выпуклый.\n");
//     } else {
//         printf("Многоугольник невыпуклый.\n");
//     }

    
//     free(points);
//     return 0;
// }
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fprintf_func(void **f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = vfprintf(*f, s, valist);
    va_end(valist);
    return res;
}

int sprintf_func(void **f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = vsprintf(*f, s, valist);
    if (res >= 0) {
        char *c = *f;
        c += res;
        *f = c;
    }
    va_end(valist);
    return res;
}

int vfprintf_func(void **f, const char *s, va_list valist) {
    return vfprintf(*f, s, valist);
}

int vsprintf_func(void **f, const char *s, va_list valist) {
    int res = vsprintf(*f, s, valist);
    if (res >= 0) {
        char *c = *f;
        c += res;
        *f = c;
    }
    return res;
}

typedef int (*two)(void **f, const char *s, va_list valist);
typedef int (*one)(void **f, const char *s, ...);

int translate_to_ss(void **f, one p, int base, int val, char start) {
    if (val == 0)
        return p(f, "0");

    if (base < 2 || base > 36)
        base = 10;

    if (val < 0) {
        p(f, "-");
        val *= -1;
    }

    int i = 0;
    char str[128];
    while (val > 0) {
        int remainder = val % base;
        char c = remainder <= 9 ? '0' + remainder : start + remainder - 10;
        str[i] = c;
        val /= base;
        i++;
    }
    for (int j = 0; j < i / 2; j++) {
        char c = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = c;
    }
    str[i] = 0;
    p(f, "%s", str);
    return 0;
}

int romanian_digist(void **f, one p, int n) {
    if (n == 0)
        return p(f, "0");

    if (n < 0) {
        n *= -1;
        p(f, "-");
    }

    int ns[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
    char *s[] = {"I",  "IV", "V",  "IX", "X",  "XL", "L",
                 "XC", "C",  "CD", "D",  "CM", "M"};
    for (int i = sizeof(s) / sizeof(s[0]) - 1; i >= 0; i--) {
        int cnt = n / ns[i];
        n = n % ns[i];
        for (int j = 0; j < cnt; j++) {
            p(f, "%s", s[i]);
        }
    }
    return 0;
}

int parse_digit(int *digit, int base, char c, char start) {
    int limit_0 = base < 10 ? base : 10;
    if ('0' <= c && c <= '0' + limit_0 - 1) {
        *digit = c - '0';
        return 0;
    }
    if (start <= c && c <= start + base - 11) {
        *digit = c - start + 10;
        return 0;
    }
    return 1;
}

int print_from_base(void **f, one p, int base, const char *s, char start) {
    if (base < 2 || base > 36)
        base = 10;
    bool neg = *s == '-';
    if (neg)
        s++;

    int digit = 0;
    int num = 0;
    for (; *s; s++) {
        if (parse_digit(&digit, base, *s, start))
            return p(f, "nan");
        int prev = num;
        num = num * base + digit;
        if (num < prev)
            return p(f, "overflow");
    }
    num = neg ? -num : num;
    p(f, "%d", num);
    return 0;
}

int zeckendorf_task(void **f, one p, unsigned int n) {
    int size = 16;
    unsigned int *fib = malloc(size * sizeof(unsigned int));
    if (!fib)
        return 1;
    fib[0] = 1;
    fib[1] = 2;
    int i = 2;
    while (1) {
        if (fib[i - 2] + fib[i - 1] > n)
            break;
        if (i >= size) {
            size *= 2;
            unsigned int *tmp = realloc(fib, size * sizeof(unsigned int));
            if (!tmp) {
                free(fib);
                return 1;
            }
            fib = tmp;
        }
        fib[i] = fib[i - 2] + fib[i - 1];
        i++;
    }
    for (int j = i - 1; j >= 0; j--) {
        if (n >= fib[j]) {
            n -= fib[j];
            fib[j] = 1;
        } else {
            fib[j] = 0;
        }
    }
    for (int j = 0; j < i; j++)
        p(f, "%d", fib[j]);
    p(f, "1");
    free(fib);
    return 0;
}


int print_bytes(void **f, one p, const void *mem, int size) {
    const unsigned char *c = mem;
    for (int i = 0; i < size; i++) {
        p(f, "%x", *c);
        if (i < size - 1)
            p(f, " ");
        c++;
    }
    return 0;
}


int choice_flag(void **f, one p, two vp, const char *s, va_list valist) {
    int len = strlen(s) + 1;
    char *str = malloc(len);
    if (!str)
        return 1;
    memcpy(str, s, len);
    char *ptr = str;
    while (str) {
        char *prev = str;
        str = strchr(str + 1, '%');
        if (str)
            *str = 0;

        if (strncmp(prev, "%Ro", 3) == 0) {
            int n = va_arg(valist, int);
            romanian_digist(f, p, n);
            prev += 3;
        }
        if (strncmp(prev, "%Zr", 3) == 0) {
            unsigned int n = va_arg(valist, unsigned int);
            zeckendorf_task(f, p, n);
            prev += 3;
        }
        if (strncmp(prev, "%Cv", 3) == 0) {
            int n = va_arg(valist, int);
            int base = va_arg(valist, int);
            translate_to_ss(f, p, base, n, 'a');
            prev += 3;
        }
        if (strncmp(prev, "%CV", 3) == 0) {
            int n = va_arg(valist, int);
            int base = va_arg(valist, int);
            translate_to_ss(f, p, base, n, 'A');
            prev += 3;
        }
        if (strncmp(prev, "%to", 3) == 0) {
            char *s = va_arg(valist, char *);
            int base = va_arg(valist, int);
            print_from_base(f, p, base, s, 'a');
            prev += 3;
        }
        if (strncmp(prev, "%TO", 3) == 0) {
            char *s = va_arg(valist, char *);
            int base = va_arg(valist, int);
            print_from_base(f, p, base, s, 'A');
            prev += 3;
        }
        if (strncmp(prev, "%mi", 3) == 0) {
            int32_t v = va_arg(valist, int32_t);
            print_bytes(f, p, &v, sizeof(v));
            prev += 3;
        }
        if (strncmp(prev, "%mu", 3) == 0) {
            uint32_t v = va_arg(valist, uint32_t);
            print_bytes(f, p, &v, sizeof(v));
            prev += 3;
        }
        if (strncmp(prev, "%md", 3) == 0) {
            double v = va_arg(valist, double);
            print_bytes(f, p, &v, sizeof(v));
            prev += 3;
        }
        if (strncmp(prev, "%mf", 3) == 0) {
            float v = va_arg(valist, double);
            print_bytes(f, p, &v, sizeof(v));
            prev += 3;
        }

        va_list copy;
        va_copy(copy, valist);
        vp(f, prev, copy);
        va_end(copy);

        if (prev[0] == '%') {
            for (; *prev != 0; prev++) {
                *prev = tolower(*prev);
                if (*prev == 'd' || *prev == 'u' || *prev == 'x' ||
                    *prev == 'o' || *prev == 'c') {
                    va_arg(valist, int);
                    break;
                }
                if (*prev == '*') {
                    va_arg(valist, int);
                    continue;
                }
                if (*prev == 'f') {
                    va_arg(valist, double);
                    break;
                }
                if (*prev == 'l') {
                    va_arg(valist, long);
                    break;
                }
                if (*prev == 'p' || *prev == 's') {
                    va_arg(valist, void *);
                    break;
                }
            }
        }
        if (str)
            str[0] = '%';
    }

    free(ptr);
    return 0;
}

int overfprintf(FILE *f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = choice_flag((void *)&f, fprintf_func, vfprintf_func, s, valist);
    va_end(valist);
    return res;
}

int oversprintf(char *f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = choice_flag((void *)&f, sprintf_func, vsprintf_func, s, valist);
    va_end(valist);
    return res;
}


int main() {
    overfprintf(stdout, "Original digits: [%d] [%d] [%d]\n", 10, 100, 1919);
    overfprintf(stdout, "Roman digist: [%Ro] [%Ro] [%Ro]\n", 10, 100, 1919);
    printf("\n");
    
    overfprintf(stdout, "Original digits: [%d] [%d] [%d]\n", 10, 100, 1919);
    overfprintf(stdout, "Zeckendorf: [%Zr] [%Zr] [%Zr]\n", 10, 100, 1919);
    printf("\n");

    overfprintf(stdout, 
    "Original digits:\n[%d] в [%d]сс\n[%d] в [%d]cc\n[%d] в [%d]cc\n[%d] в [%d]сс\n",
    100, 30, 5, 2, 24, 11, -4, 30);
    overfprintf(stdout, "ToBase: [%Cv] [%CV] [%CV] [%CV]\n", 100, 30, 5, 2, 24, 11,
                -4444444444, 30);
    printf("\n");

    overfprintf(stdout, 
    "Original digits:\n[%s] из 2сс\n[%s] из 14сс\n[%s] из 20сс\n[%s] из 36сс\n",
    "00101", "23A", "FFFFFFFFFFFFFFFFFFFFFFFF", "ABCDEFG");
    overfprintf(stdout, "FromBase: [%to] [%TO] [%TO] [%TO]\n", "00101", 2, "23A", 14,
                "FFFFFFFFFFFFFFFFFFFFFFFF", 20, "ABCDEFG", 36);
    printf("\n");

    char buffer[128] = {0};
    oversprintf(buffer, "Bytes: [%mi] [%mi] [%mu] [%mf] [%md]\n", INT64_MAX, INT16_MIN,
                INT64_MAX, INT64_MAX, INT64_MAX);
    fprintf(stdout, "%s", buffer);
}
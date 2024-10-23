#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef int (*scan)(void **f, const char *s, ...);
typedef int (*vscan)(void **f, const char *s, va_list valist);
typedef int (*seekr)(void **f, int prev);

int for_count(int *res, const char *s, char c) {
    for (; *s != 0; s++)
        (*res) += (*s == c);
    return 0;
}

int advance_string(void **f, const char *s) {
    int len = strlen(s);
    int cnt;
    for_count(&cnt, s, '%');
    char *s2 = malloc(len + cnt + 3);
    if (!s2)
        return -1;
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] == '%') {
            s2[j] = '%';
            j++;
            s2[j] = '*';
            j++;
            continue;
        }
        s2[j] = s[i];
        j++;
    }

    j += 2;
    int n = 0;
    do {
        s2[j - 2] = '%';
        s2[j - 1] = 'n';
        s2[j] = 0;
        sscanf(*f, s2, &n);
        j--;
    } while (n == 0 && j >= 2);

    char *c = *f;
    c += n;
    *f = c;
    free(s2);
    return 0;
}


int fscanf_func(void **f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = vfscanf(*f, s, valist);
    va_end(valist);
    return res;
}

int sscanf_func(void **f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = vsscanf(*f, s, valist);
    advance_string(f, s);
    va_end(valist);
    return res;
}

int vfscanf_func(void **f, const char *s, va_list valist) {
    return vfscanf(*f, s, valist);
}

int vsscanf_func(void **f, const char *s, va_list valist) {
    int res = vsscanf(*f, s, valist);
    advance_string(f, s);
    return res;
}

int fseek_func(void **f, int prev) {
    ungetc(prev, *f);
    return 0;
}

int sseek_func(void **f, int prev) {
    (void)prev;
    char *c = *f;
    c--;
    *f = c;
    return 0;
}

int parse_digit(int *digit, int base, char c, char start) {
    if ('0' <= c && c <= '0' + base - 1) {
        *digit = c - '0';
        return 0;
    }
    if (start <= c && c <= start + base - 11) {
        *digit = c - start + 10;
        return 0;
    }
    return 1;
}

int romanian_task(void **f, scan scan, seekr seek, int *n) {
    char c;
    if (scan(f, "%c", &c) <= 0)
        return 0;
    while (isspace(c)) {
        if (scan(f, "%c", &c) <= 0)
            return 0;
    }
    bool neg = c == '-';
    if (neg)
        if (scan(f, "%c", &c) <= 0)
            return 0;
    if (c == '0') {
        *n = 0;
        return 1;
    }

    int ns[] = {1, 5, 10, 50, 100, 500, 1000};
    char s[] = {'I', 'V', 'X', 'L', 'C', 'D', 'M'};

    int cnt = sizeof(s) / sizeof(s[0]);
    int num = 0;
    int prev = 2000;
    int i;
    while (c) {
        for (i = 0; i < cnt; i++)
            if (c == s[i])
                break;
        if (c != s[i]) {
            seek(f, c);
            break;
        }
        if (prev < ns[i])
            num -= 2 * prev;

        num += ns[i];
        prev = ns[i];

        if (scan(f, "%c", &c) <= 0)
            break;
    }

    num = neg ? -num : num;
    *n = num;
    return 1;
}

int zeckendorf_task(void **f, scan scan, seekr seek, unsigned int *n) {
    char c;
    char c0 = '0';
    if (scan(f, "%c", &c) <= 0)
        return 0;
    while (isspace(c)) {
        if (scan(f, "%c", &c) <= 0)
            return 0;
    }

    int b = 1;
    int t = 1;
    int a = 1;

    int num = 0;

    while (c) {
        if (c != '0' && c != '1') {
            seek(f, c);
            return 0;
        }
        if (c == '1' && c0 == '1') {
            *n = num;
            return 1;
        }

        if (c == '1') {
            num += a;
        }

        t = a;
        a += b;
        b = t;

        c0 = c;
        if (scan(f, "%c", &c) <= 0)
            break;
    }
    return 0;
}



int analyse_ss(void **f, scan scan, seekr seek, int *n, int base,
              char start) {
    if (base < 2 || base > 36)
        base = 10;

    char c;
    if (scan(f, "%c", &c) <= 0)
        return 0;
    while (isspace(c)) {
        if (scan(f, "%c", &c) <= 0)
            return 0;
    }
    bool neg = c == '-';
    if (neg)
        if (scan(f, "%c", &c) <= 0)
            return 0;

    int digit = 0;
    int num = 0;
    while (c) {
        if (parse_digit(&digit, base, c, start)) {
            seek(f, c);
            break;
        }
        num = num * base + digit;
        if (scan(f, "%c", &c) <= 0)
            break;
    }
    num = neg ? -num : num;
    *n = num;
    return 1;
}

int scanf_choice_flag(void **f, scan scan, vscan vscan, seekr seek,
                  const char *s, va_list valist) {
    int len = strlen(s) + 1;
    char *str = malloc(len);
    if (!str)
        return -1;
    memcpy(str, s, len);
    char *ptr = str;
    char flags[] = {'c', 's', '[', 'd', 'i', 'u', 'o', 'x', 'X', 'n',
                    'a', 'A', 'e', 'E', 'f', 'F', 'g', 'G', 'p'};
    int flags_cnt = sizeof(flags) / sizeof(flags[0]);
    int cnt = 0;

    while (str) {
        char *prev = str;
        str = strchr(str + 1, '%');
        if (str)
            *str = 0;

        if (strncmp(prev, "%Ro", 3) == 0) {
            int *n = va_arg(valist, int *);
            if (!romanian_task(f, scan, seek, n))
                break;
            prev += 3;
            cnt++;
        }
        if (strncmp(prev, "%Zr", 3) == 0) {
            unsigned int *n = va_arg(valist, unsigned int *);
            if (!zeckendorf_task(f, scan, seek, n))
                break;
            prev += 3;
            cnt++;
        }
        if (strncmp(prev, "%Cv", 3) == 0) {
            int *n = va_arg(valist, int *);
            int base = va_arg(valist, int);
            if (!analyse_ss(f, scan, seek, n, base, 'a'))
                break;
            prev += 3;
            cnt++;
        }
        if (strncmp(prev, "%CV", 3) == 0) {
            int *n = va_arg(valist, int *);
            int base = va_arg(valist, int);
            if (!analyse_ss(f, scan, seek, n, base, 'A'))
                break;
            prev += 3;
            cnt++;
        }
        if (prev[0] != '%' || (prev[0] == '%' && prev[1] == '%')) {
            bool end = false;
            char c;

            for (; prev < str; prev++) {
                if (!scan(f, "%c", &c)) {
                    end = true;
                    break;
                }
                if (*prev != c) {
                    end = true;
                    break;
                }
            }
            if (str)
                str[0] = '%';
            if (end)
                break;
            continue;
        }

        va_list copy;
        va_copy(copy, valist);
        int res = vscan(f, prev, copy);
        va_end(copy);

        if (prev[0] == '%' && prev[1] != '%' && prev[1] != '*' && res < 1)
            break;

        cnt++;

        if (prev[0] == '%') {
            for (; *prev != 0; prev++) {
                if (*prev == '*') {
                    break;
                }
                for (int i = 0; i < flags_cnt; i++) {
                    if (*prev == flags[i]) {
                        va_arg(valist, void *);
                        break;
                    }
                }
            }
        }
        if (str)
            str[0] = '%';
    }

    free(ptr);
    return cnt;
}

int overfscanf_func(FILE *f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = scanf_choice_flag((void *)&f, fscanf_func, vfscanf_func,
                            fseek_func, s, valist);
    va_end(valist);
    return res;
}

int oversscanf_func(char *f, const char *s, ...) {
    va_list valist;
    va_start(valist, s);
    int res = scanf_choice_flag((void *)&f, sscanf_func, vsscanf_func,
                            sseek_func, s, valist);
    va_end(valist);
    return res;
}

int main() {
    int a, b, c;
    float f;
    char *s = "X -XL XIII : 0.4";
    printf("Original string: %s\n", s);
    int r = oversscanf_func(s, "%Ro %Ro %Ro : %f", &a, &b, &c, &f);
    printf("%d\t%d %d %d %f\n", r, a, b, c, f);
    printf("\n");

    s = "ffff, 1010, DE : 0.1";
    printf("Original string: %s\n", s);
    r = oversscanf_func(s, "%Cv, %Cv, %CV : %f", &a, 16, &b, 2, &c, 36, &f);
    printf("%d\t%d %d %d %f\n", r, a, b, c, f);
    printf("\n");

    s = "00101000011 : 123";
    printf("Original string: %s\n", s);
    r = oversscanf_func(s, "%Zr : %d", &a, &b);
    printf("%d\t%d %d\n", r, a, b);

    
    return 0;
}
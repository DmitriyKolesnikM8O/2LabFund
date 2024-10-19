#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void compute_prefix_function(const char *pattern, int *pi, int m) {
    int k = 0;
    pi[0] = 0;
    
    for (int i = 1; i < m; i++) {
        while (k > 0 && pattern[k] != pattern[i]) {
            k = pi[k - 1];
        }
        if (pattern[k] == pattern[i]) {
            k++;
        }
        pi[i] = k;
    }
}

int kmp_search(const char *text, const char *pattern, int line_number) {
    int n = strlen(text);
    int m = strlen(pattern);


    int *pi = (int *)malloc(m * sizeof(int));
    if (pi == NULL) {
        printf("Не удалось выделить память для массива префиксов\n");
        return -1;
    }
    compute_prefix_function(pattern, pi, m);

    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k > 0 && pattern[k] != text[i]) {
            k = pi[k - 1];
        }
        if (pattern[k] == text[i]) {
            k++;
        }
        if (k == m) {

            printf("Найдено вхождение в строке %d на позиции %d\n", line_number, i - m + 2);
            k = pi[k - 1];
        }
    }

    free(pi);

    return 0;
}


int find_substring_in_files(const char *substring, int num_files, ...) {
    va_list args;
    va_start(args, num_files);

    for (int i = 0; i < num_files; i++) {
        const char *file_path = va_arg(args, const char *);
        FILE *file = fopen(file_path, "r");
        if (!file) {
            printf("Ошибка открытия файла: %s\n", file_path);
            return -1;
        }

        printf("Поиск в файле: %s\n", file_path);
        char line[1024];
        int line_number = 1;

        while (fgets(line, sizeof(line), file)) {
            kmp_search(line, substring, line_number);
            line_number++;
        }

        fclose(file);
    }

    va_end(args);

    return 0;
}

int main() {
    const char *substring = "пример";

    if (!find_substring_in_files(substring, 2, "fileInFor3TaskOne.txt", "fileInFor3TaskTwo.txt")) {
        printf("У меня не получилось");
    }

    return 0;
}

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define N 5

typedef char BOOL;

// вычисляет логическую функцию
int logic(BOOL *a, int size) {
    BOOL logic = 1;
    for (int i = 0; i < size; i++) {
        logic = logic && a[i];
    }
    return logic;
}

// вычисляет и распечатывает результаты
void print_result(BOOL *a, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = a[i] ? 1 : 0;
        printf("%d ", a[i]);
    }
    printf("%s\n", logic(a, size)? "True" : "False");
}

int main() {
    BOOL a[N];
    
    for (int i = 0; i < N; i++) {
        a[i] = fork();
    }

    print_result(a, N);

    // ожидает завершения всех потомков текущего процесса
    while(wait(NULL) > 0);
}
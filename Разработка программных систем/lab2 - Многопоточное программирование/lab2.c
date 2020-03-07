#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

char* filename1 = NULL;
char* filename2 = NULL;
char name_changed_1 = 0;
char name_changed_2 = 0;
char end = 0;

// перевод в нижний регистр
void* lower(void* attr) {
    // пока внешним потоком не будет установлен флаг окончания выполнения
    while (!end) {
        // открытие файла для чтения
        FILE* in = fopen(filename2, "r");
        if (in == NULL) {
            return (void*)-1;
        }

        // чтение по одному символу и вывод с переходом на новую строку,
        // пока файл не закончится или вне потока 
        // не будет установлен один из флагов name_changed или end
        char symbs[2];
        symbs[1] = '\n';
        size_t read_num = 1;
        while (read_num >= 1 && name_changed_1 == 0 && end == 0) {
            read_num = fread(&symbs[0], 1, 1, in);
            symbs[0] = tolower(symbs[0]);
            write(1, &symbs, 2);
            sleep(1);
        }
       
        // снимаем флаг изменения имени файла 
        name_changed_1 = 0;

        // закрытие файла
        int is_closed = fclose(in);
        if (is_closed == EOF) {
            return (void*)-1;
        }
    }

    pthread_exit(0);
}

// перевод в верхний регистр
void* upper(void* attr) {
    // пока внешним потоком не будет установлен флаг окончания выполнения
    while (!end) {
        // открытие файла для чтения
        FILE* in = fopen(filename1, "r");
        if (in == NULL) {
            return (void*)-1;
        }

        // чтение по одному символу и вывод с переходом на новую строку,
        // пока файл не закончится или вне потока 
        // не будет установлен один из флагов name_changed или end
        char symbs[2];
        symbs[1] = '\n';
        size_t read_num = 1;
        while (read_num >= 1 && name_changed_2 == 0 && end == 0) {
            read_num = fread(&symbs[0], 1, 1, in);
            symbs[0] = toupper(symbs[0]);
            write(1, &symbs, 2);
            sleep(1);
        }

        // снимаем флаг изменения имени файла 
        name_changed_2 = 0;
        
        // закрытие файла
        int is_closed = fclose(in);
        if (is_closed == EOF) {
            return (void*)-1;
        }
    }

    pthread_exit(0);
}

// удалаление считанного перевода строки на конце названия файле
void remove_enter(char* name) {
    int i;
    for (i = 0; i < strlen(name); i++) {}
    name[i-1] = '\0';
}

// контроль потоков и считывание новых имен файлов
int main() {
   // чтение названий файла
    size_t size_of_line = 0;
    // статический буфер
    ssize_t read_num = getline(&filename1, &size_of_line, stdin);
    read_num = getline(&filename2, &size_of_line, stdin);

    // удаление лишних \n из строк с названиями файлов
    remove_enter(filename1);
    remove_enter(filename2);

    // задание атрибутов создаваемых потоков
    pthread_attr_t pattr;
    pthread_attr_init(&pattr);
    pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE);

    // создание и запуск потоков
    pthread_t pid1, pid2;
    pthread_create(&pid1, &pattr, upper, filename1);
    pthread_create(&pid2, &pattr, lower, filename2);

    // чтение новых имен файлов, пока это возможно
    // и изменение их в глобальных переменных
    char* filename_swp = NULL;
    while (getline(&filename_swp, &size_of_line, stdin) > 0) {
        remove_enter(filename_swp);
        if (strcmp(filename_swp,"stop") == 0) {
            end = 1;
            free(filename_swp);
            free(filename1);
            free(filename2);
            pthread_attr_destroy(&pattr);
            pthread_exit(0);
        }
        strcpy(filename2, filename1);
        strcpy(filename1, filename_swp);
        name_changed_1 = 1;
        name_changed_2 = 1;
    }

    // ожидание завершения дочерних потоков
    int retval1 = 0, retval2 = 0;
    pthread_join(pid1, (void*)&retval1);
    pthread_join(pid2, (void*)&retval2);

    // освобождение выделенной памяти
    free(filename1);
    free(filename2);
    pthread_attr_destroy(&pattr);
    pthread_exit(0);
}

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include "client.h"
#include "terminal.h"

#define TRUE 1
#define FALSE 0

#define INSIDE 0
#define OUTSIDE 1

int server_go;
 
// структура состояния и конфигурации лифта
typedef struct {
    double v_max;          // скорость лифта, когда он едет в секундах
    const int floors;      // количество этажей, на которое расчитан лифт
    const int persons_max; // максимально допустимое количество человек в лифте одновременно
    int floor;             // текущий этаж пребывания лифта
    char moving;           // двигается ли лифт
    int person_num;        // количество человек внутри
} Elevator;

// лифт и его начальное состояние
Elevator lift = {10.0, 10, 5, 1, FALSE, 0};

// добавляет person_num человек в лифт, если это возможно
char add_persons(int person_num) {
    // если суммарное количество больше допустимого, ошибка
    if ((lift.person_num + person_num) > lift.persons_max)
    {
        printf("Too many persons, sorry\n");
        return FALSE;
    }

    lift.person_num += person_num;
    return TRUE;
}

// удаляет person_num человек из лифта, если это возможно
char remove_persons(int person_num) {
    // если попытка удалить больше, чем есть, ошибка
    if (person_num > lift.person_num)
    {
        printf("Error in num of pearsons to leave, sorry\n");
        return FALSE;
    }

    lift.person_num -= person_num;
    return TRUE;
}

// печатает текущее состояние лифта
void print_state() {
    gotoxy(1, 1);
    for (int i = 1; i <= lift.floors; i++)
    {
        if (i != lift.floor)
        {
            set_display_atrib(B_BLACK);
            printf("-");
        }
        else
        {
            set_display_atrib(B_RED);
            printf("%d", lift.person_num);
        }
    }
    resetcolor();
    printf("\n");
}

// отправляет лифт на этаж destination
char go(int destination, char from) {
    if (destination < 1 || destination > lift.floors)
    {
        printf("Unreachable destination: %d\n", destination);
        return FALSE;
    }

    // сколько этажей нужно проехать
    int steps = abs(destination - lift.floor);

    // вниз или вверх двигаться
    int step = (destination > lift.floor) ? 1 : -1;

    // движение
    for (int i = 0; i < steps; i++)
    {
        lift.moving = TRUE;
        sleep(10 / lift.v_max);
        lift.floor += step;
    }

    // если лифт вызвали снаружи, то по прибытии 1 человек заходит
    // иначе - 1 выходит
	if (from == INSIDE) {
		remove_persons(1);
	} else if (from == OUTSIDE) {
		add_persons(1);
	}
    lift.moving = FALSE;

    return TRUE;
}

// функция потока вызовов лифта изнутри
void *calls(void *attr) {
    int num;
    while (1)
    {
        gotoxy(1, 2);
        printf("  \b\b");

        scanf("%d", &num);
        while(lift.moving) {}
        go(num, INSIDE);
    }
}

// функция потока отображения состояния
void *view(void *attr) {
    while (1)
    {
        print_state();
        sleep(1);
    }
}

// функция потока получения команды с сервера и ее выполнения
void *message(void *attr) {
    // на сигнал SIGINT (Ctrl+C) - вызов функции завершения работы клиента
    signal(SIGINT, stop_client);

    // открытие сокета
    struct sockaddr_in cname;
    s = create_socket(&cname);
    if (s == -1) {
        printf("Неудача при открытии сокета\n");
        return (void*)(-1);
    }

    // инициализация структуры sockaddr_in сервера
    struct sockaddr_in sname;
    set_sockaddr_of_srv(&sname);

    // соединение
    int c = connect(s, (struct sockaddr *)&sname, sizeof(sname));
    if (c == -1) {
        printf("Не удается получить доступ к серверу\n");
        return (void*)(-1);
    }

    // цикл бесконечного считывания команд сервера и их выполнения
    while (1) {
        // получение сообщения с сервера
        char buf[BUF_SIZE];
        int get_len = recv(s, buf, BUF_SIZE, 0);
        if (get_len == -1) {
            printf("Сообщение с сервера не может быть получено\n");
            return (void*)(-1);
        }

        // печать полученного сообщения и отправка лифта 
        // в пункт назначения, как только он освободится
        if (get_len != 0) {
            gotoxy(1,3);
            printf("Получено сообщение:\n");
        	gotoxy(1, 4);
        	printf("  \b\b");

            write (1, buf, get_len);
            while(lift.moving) {}
            go(atoi(buf), OUTSIDE);
            printf("\n");
        }
    }
}

// создание потоков и управление ими
int main(int argc, char* argv[]) {
    // очистка экрана
    clrscr();

    // установление порта по первому аргументу,
    // скорости в соответствии со вторым аргументом программы,
    // если они были поданы, иначе по-умолчанию остается v=1, port=1235
    if (argc == 3) {
        C_PORT = atoi(argv[1]);
        lift.v_max = atof(argv[2]);
    }

    // задание атрибутов потоков
    pthread_attr_t pattr;
    pthread_attr_init(&pattr);
    pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE);

    // создание и запуск потоков
    pthread_t calls_pid, view_pid, message_pid;
    pthread_create(&calls_pid, &pattr, calls, NULL);
    pthread_create(&view_pid, &pattr, view, NULL);
    pthread_create(&message_pid, &pattr, message, NULL);

    // ожидание завершения дочерних потоков
    int retval1 = 0, retval2 = 0;
    pthread_join(calls_pid, (void *)&retval1);
    pthread_join(view_pid, (void *)&retval2);
}

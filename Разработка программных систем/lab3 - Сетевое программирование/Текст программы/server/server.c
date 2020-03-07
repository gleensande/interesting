#include <pthread.h>
#include "server.h"

char floor_num[3];
char lift_num = 0;

// создание сокета и его именование
int create_socket(struct sockaddr_in* sname) {
    // открытие сокета
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        printf("Не удалось открыть сокет\n");
        return -1;
    }

    // cоздание и заполнение структуры sockaddr
    memset ((char *)sname, '\0', sizeof(sname));
    sname->sin_family = AF_INET;
    sname->sin_port = S_PORT;

    // для переиспользования адреса
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
        printf("Setsockopt(SO_REUSEADDR) не может сработать\n");
    }

    // установление адреса сервера
    sname->sin_addr.s_addr = INADDR_ANY;

    // именование сокета с помощью структуры sockaddr
    int binded = bind(s, (struct sockaddr *)sname, sizeof(*sname));
    if (binded == -1) {
        printf("Не удалось именовать сокет\n");
        return -1;
    }

    return s;
}

// завершение работы сервера
void stop_server(int n) {
    printf("Завершение работы сервера\n");
    if (s != 0) {
        shutdown(s, BOTH);
    }
    if (new_s1!= 0) {
        shutdown(new_s1, BOTH);
    }
    if (new_s2!= 0) {
        shutdown(new_s2, BOTH);
    }
    remove(S_NAME);
    exit(0);
}

void* lift1(void* attr) {
    // принятие соединения от клиента
    int addrlen = 0;
    new_s1 = accept(s, (struct sockaddr *)&sname, &addrlen);
    if (new_s1 == -1) {
        printf("Неудача при попытке принять соединение от клиента\n");
        return (void*)(-1);
    }
	while(1) {
		if (lift_num == 1) {
			lift_num = 3;
        	write(new_s1, floor_num, sizeof(floor_num));
		}
	}
}

void* lift2(void* attr) {
    // принятие соединения от клиента
    int addrlen = 0;
    new_s2 = accept(s, (struct sockaddr *)&sname, &addrlen);
    if (new_s2 == -1) {
        printf("Неудача при попытке принять соединение от клиента\n");
        return (void*)(-1);
    }
	while(1) {
		if (lift_num == 2) {
			lift_num = 0;
        	write(new_s2, floor_num, sizeof(floor_num));
		}
	}
}

int main() {
    // на сигнал SIGINT (Ctrl+C) - вызов функции завершения работы сервера
    signal(SIGINT, stop_server);
    
    // открытие сокета
    s = create_socket(&sname);
    if (s == -1) {
        printf("Неудача при открытии сокета\n");
        return -1;
    }

    // прослушивание входящего потока данных
    int l = listen(s, 1);
    if (l == -1) {
        printf("Неудача при попытке слушать\n");
        return -1;
    }
    printf("Сервер слушает на %d:%d\n", sname.sin_addr.s_addr, S_PORT);

    // задание атрибутов потоков
    pthread_attr_t pattr;
    pthread_attr_init(&pattr);
    pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE);

    // создание и запуск потоков
    pthread_t lift1_pid, lift2_pid;
    pthread_create(&lift1_pid, &pattr, lift1, NULL);
    pthread_create(&lift2_pid, &pattr, lift2, NULL);

    // прослушивание вызовов снаружи лифтов
    int num;
    while (1)
    {
        printf("  \b\b");

        scanf("%d", &num);
        snprintf(floor_num, 3, "%d%c", num, '\0');
		if (lift_num == 0) {
			lift_num = 1;
		} else {
			lift_num = 2;
		}
		printf("Отправка: %s на лифт №%d\n", floor_num, lift_num);
    }

    // ожидание завершения дочерних потоков
    int retval1 = 0, retval2 = 0;
    pthread_join(lift1_pid, (void *)&retval1);
    pthread_join(lift2_pid, (void *)&retval2);
}


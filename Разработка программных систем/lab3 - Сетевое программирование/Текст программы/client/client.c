#include "client.h"

// создание сокета и его именование
int create_socket(struct sockaddr_in* cname) {
    // открытие сокета
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        printf("Не удалось открыть сокет\n");
        return -1;
    }

    // cоздание и заполнение структуры sockaddr
    memset((char *)cname, '\0', sizeof(*cname));
    cname->sin_family = AF_INET;
    if (C_PORT == 0) {
        C_PORT = 1235;
        cname->sin_port = C_PORT;
    }

    // установление адреса клиента
    cname->sin_addr.s_addr = INADDR_ANY;

    // для переиспользования адреса
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
        printf("Setsockopt(SO_REUSEADDR) не может сработать\n");
    }

    // именование сокета с помощью структуры sockaddr
    int binded = bind(s, (struct sockaddr *)cname, sizeof(*cname));
    if (binded == -1) {
        printf("Не удалось именовать сокет\n");
        printf("err: %s\n", strerror(errno));
        return -1;
    }

    return s;
}

// инициализация структуры sockaddr_in сервера
void set_sockaddr_of_srv(struct sockaddr_in* sname) {
    memset((char *)sname, '\0', sizeof(sname));

    sname->sin_family = AF_INET;
    sname->sin_port = S_PORT;

    struct hostent* hp = gethostbyname(S_HOST);
    memcpy((void*)&(sname->sin_addr), hp->h_addr, hp->h_length);
}

// завершение работы клиента
void stop_client(int n) {
    printf("\033[%d;%dH", 5, 1);
    printf("Завершение работы клиента\n");
    if (s != 0) {
        shutdown(s, BOTH);
    }
    remove(C_NAME);
    exit(0);
}

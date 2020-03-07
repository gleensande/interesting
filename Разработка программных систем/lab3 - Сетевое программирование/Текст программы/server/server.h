// для печати
#include <stdio.h>
#include <unistd.h>

// для tcp и sockaddr_in
#include <netinet/in.h>

// для memset
#include <memory.h>

// сокеты
#include <sys/types.h>
#include <sys/socket.h>

// сигналы + exit
#include <signal.h>
#include <stdlib.h>

#ifndef LIFT_SERVER_H
#define LIFT_SERVER_H

#define S_NAME "/tmp/srv.sock"  // название сокета в файловой системе
#define S_PORT 1234             // порт, где будет работать сервер

#define BOTH 2                  // аргумент shutdown для закрытия на чтение и на запись

// сокеты программы
int s, new_s1, new_s2;

// структура имени сокета
struct sockaddr_in sname;

// создание сокета и его именование
int create_socket(struct sockaddr_in* sname);

// завершение работы сервера
void stop_server(int n);

#endif  // LIFT_SERVER_H

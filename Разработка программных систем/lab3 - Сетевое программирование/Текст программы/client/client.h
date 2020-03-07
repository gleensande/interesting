// для печати
#include <stdio.h>
#include <unistd.h>

// для tcp и sockaddr_in
#include <netinet/in.h>

// для memset
#include <memory.h>

// для hostent
#include <netdb.h>

// для errno
#include <errno.h>

// сокеты
#include <sys/types.h>
#include <sys/socket.h>

// сигналы + exit
#include <signal.h>
#include <stdlib.h>

#ifndef LIFT_CLIENT_H
#define LIFT_CLIENT_H

#define C_NAME "/tmp/cl.sock"   // название сокета в файловой системе

#define S_HOST "localhost"      // хост удаленного сервера
#define S_PORT 1234             // порт удаленного сервера

#define BOTH 2                  // аргумент shutdown для закрытия на чтение и на запись

#define BUF_SIZE 64             // буфер для приема сообщения с сервера

int s;                          // сокет программы

int C_PORT;                     // порт, где будет работать клиент

// создание сокета и его именование
int create_socket(struct sockaddr_in* cname);

// инициализация структуры sockaddr_in сервера
void set_sockaddr_of_srv(struct sockaddr_in* sname);

// завершение работы клиента
void stop_client(int n);

#endif  // LIFT_CLIENT_H
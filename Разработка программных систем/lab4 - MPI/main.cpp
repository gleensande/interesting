#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>
#include "main.hpp"

using namespace std;

// возможность повторного входа в функцию разными процессами
#define _REENTRANT

// константы программы, определяемые
// из аргумeнтов командной строки
long int N; // количество элементов
long double T; // интервал времени

// установка констант из аргумeнтов программы
int get_args(int argc, char** argv) {
    if (argc < 3) {
        printf("Too few arguments\n");
        return -1;
    }

    char** endptr = NULL;
    N = strtol(argv[1], endptr, 10);
    if (endptr == &argv[1]) {
        printf("Num of elements is not a number\n");
        return -1;
    }

    if (((N - 2) % 8) != 0) {
        printf("Num of elements - 2 can't be divided by 8\n");
        return -1;
    }

    endptr = NULL;
    T = strtof(argv[2], endptr);
    if (endptr == &argv[2]) {
        printf("Time interval is not a number\n");
        return -1;
    }

    return 0;
}

// создание скрипта для отрисовки в gnuplot
int make_script() {
    // имя файла со скриптом для gnuplot
    const char* filename = "script.gnu";
    
    // очистка содержимого файла
    fstream cleared_file(filename, ios::out);
    cleared_file.close();

    // открытие файла, запись туда скрипта для gnuplot и закрытие
    fstream dat_file;
    dat_file.open(filename, fstream::in | fstream::out | fstream::app);

    dat_file << "set xrange[0:" << N - 1 << "]\n";
    dat_file << "set yrange[-200:200]\n";
    dat_file << "do for [i=0:" << (int)(T / (10 * H_T)) << "]{\n";
    dat_file << "plot 'output.dat' index i using 1:2 smooth bezier\npause 0.1}\npause -1\n";

    dat_file.close();
}


int main(int argc, char** argv) {
    if (get_args(argc, argv) == -1) {
        return -1;
    }

    int myrank; // собственный идентификатор процесса
    int total;  // общее количество процессов в группе
    MPI_Status status; // статус при отправке и получении сообщений

    // запуск MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &total);

    // чтобы каждый процесс узнал свой id
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // файл, куда будут записываться данные
    fstream dat_file;

    // массивы значение на текущем и на предыдущем шаге
    // глобальные (root) и локальные (для каждого процесса)
    double *U_prev, *U;
    double *u_prev, *u;

    // коэффициент, который будет использоваться в уравнениях
    const double coeff = H_T / (R * C);

    // расчет количества данных для одного процесса
    int pcount = (N - 2) / total;           // без границ, только рассчитываемое    

    // подготовка исходныx данныx процессом root (id = 0)
    if (!myrank) {
        const char* filename_main = "output.dat";
        
        fstream cleared_file(filename_main, ios::out);
        cleared_file.close();

        dat_file.open(filename_main, fstream::in | fstream::out | fstream::app);

        // выделение памяти для массивов
        U_prev = new double[N];
        U = new double[N];

        // заполнение начальными условиями
        for (int i = 0; i < N; i++) {
            U_prev[i] = U0;
        }

        // дополнение граничными условиями
        U_prev[0] = Ua;
        //U_prev[N - 1] = Ub;  
        U_prev[N - 1] = H_T / C * ((u_prev[pcount - 2] + u_prev[pcount - 1]) / R + Ib) + u_prev[pcount - 1];      
    }


    // выделение памяти для матриц u каждого процесса
    u_prev = new double[pcount];
    u = new double[pcount];

    // для измерения времени
    clock_t start_time, end_time, all_time;
    if (!myrank) {
        start_time = clock();
    }

    // все процессы узнают соседей и получают начальные условия
    int NextProcNum = (myrank == total - 1) ? MPI_PROC_NULL : myrank + 1;
	int PrevProcNum = (myrank == 0) ? MPI_PROC_NULL : myrank - 1;

    for (int i = 0; i < pcount; i++) {
        u_prev[i] = U0;
    }

    // граничные условия для начального состояния
    double edges_prev[2], edges[2], right, left;
    edges_prev[0] = U0;
    edges_prev[1] = U0;

    if (myrank == 0) {
        edges_prev[0] = Ua;
    }

    if (myrank == total - 1) {
        //edges_prev[1] = Ub;
        edges_prev[1] = H_T / C * ((u_prev[pcount - 2] + u_prev[pcount - 1]) / R + Ib) + u_prev[pcount - 1];      

    }

    edges[0] = 0;
    edges[1] = 0;


    for (double t = H_T; t < T; t += H_T) {
        // каждый процесс считает свою часть
        u[0] = u_prev[0] + coeff * (edges_prev[0] - 2 * u_prev[0] + u_prev[1]);
        for (int i = 1; i < pcount - 1; i++) {
            u[i] = u_prev[i] + coeff * (u_prev[i - 1] - 2 * u_prev[i] + u_prev[i + 1]);
        }
        u[pcount - 1] = u_prev[pcount - 1] + 
            coeff * (u_prev[pcount - 2] - 2 * u_prev[pcount - 1] + edges_prev[1]);

        // потом отправляет и принимает edges[2]
        right = u[pcount - 1];
        left = u[0];

        // правая граница текущего процесса превращается
        // в левую границу следующего
        if (myrank == 0 ) {
            MPI_Send(
                (void*)(&right),
                1,
                MPI_DOUBLE,
                NextProcNum,
                1,
                MPI_COMM_WORLD
            );
            edges[0] = Ua;
        } 
        if (myrank == total - 1) {
            MPI_Recv(
                (void*)(&edges[0]),
                1,
                MPI_DOUBLE,
                PrevProcNum,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
            );
        } 
        if ((myrank != 0) && (myrank != total - 1)) {
            MPI_Send(
                (void*)(&right),
                1,
                MPI_DOUBLE,
                NextProcNum,
                1,
                MPI_COMM_WORLD
            );
            MPI_Recv(
                (void*)(&edges[0]),
                1,
                MPI_DOUBLE,
                PrevProcNum,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
            );
        }

        // левая граница текущего процесса превращается
        // в правую границу предыдущего
        if (myrank == 0 ) {
            MPI_Recv(
                (void*)(&edges[1]),
                1,
                MPI_DOUBLE,
                NextProcNum,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
            );
        }
        if (myrank == total - 1) {
            MPI_Send(
                (void*)(&left),
                1,
                MPI_DOUBLE,
                PrevProcNum,
                1,
                MPI_COMM_WORLD
            );
            //edges[1] = Ub;
            edges[1] = H_T / C * ((u_prev[pcount - 2] + u_prev[pcount - 1]) / R + Ib) + u_prev[pcount - 1];
        } 
        if ((myrank != 0) && (myrank != total - 1)) {
            MPI_Send(
                (void*)(&left),
                1,
                MPI_DOUBLE,
                PrevProcNum,
                1,
                MPI_COMM_WORLD
            );
            MPI_Recv(
                (void*)(&edges[1]),
                1,
                MPI_DOUBLE,
                NextProcNum,
                MPI_ANY_TAG,
                MPI_COMM_WORLD,
                &status
            );
        }

        // перезаписывает edges_prev
        edges_prev[0] = edges[0];
        edges_prev[1] = edges[1];

        // отправляет руту, тот записывает в файл
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Gather(
            (void*)u,
            pcount,
            MPI_DOUBLE,
            (void*)(U + 1),
            pcount,
            MPI_DOUBLE,
            0,
            MPI_COMM_WORLD
        );

        // дополнение граничными условиями и запись в файл
        if (!myrank) {
            // граничные условия
            U[0] = Ua;
            //U[N - 1] = Ub;
            U[N - 1] = edges[1] = H_T / C * ((u_prev[pcount - 2] + u_prev[pcount - 1]) / R + Ib) + u_prev[pcount - 1];

            // запись значений в файл
            for (int i = 0; i < N; i++) {
                dat_file << i << " ";
                dat_file << setprecision(10) << U[i] << endl;
            }
            dat_file << endl << endl;
        }

        for (int i = 0; i < pcount; i++) {
            u_prev[i] = u[i];
        }
    }

    // завершение работ root-процесса
    if (!myrank) {
        dat_file.close();

        // завершение подсчета времени
        end_time = clock();
        all_time = (end_time - start_time) / CLOCKS_PER_SEC;
        cout << all_time << "s" << endl; 

        // создание root-процессом скрипта для gnuplot
        make_script();

        // очистка памяти
        delete[] U_prev;
        delete[] U;
    }

    // завершение работы
    delete[] u_prev;
    delete[] u;
    MPI_Finalize();
    exit(0);
}


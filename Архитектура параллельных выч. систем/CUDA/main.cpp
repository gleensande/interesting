#include <stdio.h>
#include "main.hpp"
#include "matrix.hpp"
#include <stdlib.h>

// матрица состояний пластины
double p[I_MAX + 1][J_MAX + 1][K_MAX + 1];

// напечатать состояние
void print_state(int k) {
    printf("Матрица p:\n");
    for (int i = 0; i <= I_MAX; i++) {
        for (int j = 0; j <= J_MAX; j++) {
            printf("%6.lf ", p[i][j][k]);
        }
        printf("\n");
    }
    printf("\n");
}

// задать начальные условия
void begin_state() {
    // внутренние узлы из НУ
    for (int i = 1; i < I_MAX; i++) {
        for (int j = 1; j < J_MAX; j++) {
            p[i][j][0] = T_0;
        }
    }

    // верхняя и нижняя границы из ГУ
    for (int j = 0; j <= J_MAX; j++) {
        p[I_MAX][j][0] = T_BOTTOM();
        p[0][j][0] = T_UP();
    }

    // левая и правая границы из ГУ
    for (int i = 1; i <= I_MAX; i++) {
        p[i][0][0] = T_LEFT(p[i][1][0]);
        p[i][J_MAX][0] = T_RIGHT();
    }

}

void prepare_state(double* c, double* A, int size, int cmax, int k) {
    // Задание граничных условий в матрице

    // верх и низ
    // граничное условие первого рода
    for (int m = 0; m <= J_MAX; m++) {
        A[m * size + m] = 1;
        c[m * size + 0] = T_UP();

        A[(cmax - m) * size + (cmax - m)] = 1;
        c[(cmax - m) * size + 0] = T_BOTTOM();
    }

    // левая граница - каждый J_MAX + 1 -ый элемент
    // граничное условие второго рода
    for (int m = 0; m <= cmax; m += J_MAX + 1) {
        A[m * size + m] = 1;
        A[m * size + (m + 1)] = -1;
        c[m * size + 0] = 0;
    }

    // правая граница - каждый J_MAX -ый элемент
    // граничное условие первого рода
    for (int m = J_MAX; m <= cmax; m += J_MAX + 1) {
        A[m * size + m] = 1;
        c[m * size + 0] = T_RIGHT();
    }

    // неявная разностная схема
    // пропускаем все граничные условия по верху и первое по левой границе
    // в условии - останавливаемся до последней правой границы

    // коэффициенты
    double Tij_coef = D_X*D_X*D_Y*D_Y + 2*D_Y*D_Y*D_T + 2*D_X*D_X*D_T;
    double Ti_jv_coef = -D_Y*D_Y*D_T;
    double Tiv_j_coef = -D_X*D_X*D_T;
    double T_prev_coeff = D_Y*D_Y*D_X*D_X;

    for (int q = J_MAX + 2; q < cmax - J_MAX; q += J_MAX + 1 ) {
        for (int m = 0; m < J_MAX - 1; m++) {
            A[size * (q+m) + q+m ] = Tij_coef;                  // T i,j
            A[size * (q+m) + q+m+J_MAX+1] = Tiv_j_coef;         // T i+1,j
            A[size * (q+m) + q+m-J_MAX-1] =Tiv_j_coef;          // T i-1,j
            A[size * (q+m) + q+m+1] = Ti_jv_coef;               // T i,j+1
            A[size * (q+m) + q+m-1] = Ti_jv_coef;               // T i,j-1
            c[size * (q+m) + 0] = p[(q+m)/(J_MAX+1)][(q+m)%(J_MAX+1)][k-1]*T_prev_coeff;
        }
    }

}

// рассчитать состояние
void count_state(int k) {
    // ----------------- CUDA 1. Выделение памяти хоста и заполнение ее -----------------

    // расчет начальных условий
    if (k == 0) {
        begin_state();
        return;
    }

    // создание исходных матриц СЛАУ
    int size = (I_MAX + 1) * (J_MAX + 1);
    int cmax = size - 1;    // максимальный номер строки в векторе с

    double* c = (double*)malloc(sizeof(double) * size * 1);
    double* A = (double*)malloc(sizeof(double) * size * size);

    prepare_state(c, A, size, cmax, k);

    // ----------------- CUDA 2. Выделение памяти девайса и заполнение ее -----------------
    // TODO

    // --------------------- CUDA 3. Перенос данных из хоста девайсу ---------------------
    // TODO

    // ----------------------- CUDA 4. Вызов ядра для решения СЛАУ -----------------------
    //A.print(&c, "Исходное уравнение:");
    Gauss(A, A_rows, A_cols, c, c_rows, c_cols);

    // --------------------- CUDA 5. Перенос данных из девайса хосту ---------------------
    // TODO

    // перенос значений из вектора c в матрицу p
    int gi = 0;
    for (int i = 0; i < I_MAX + 1; i++) {
        for (int j = 0; j < J_MAX + 1; j++) {
            p[i][j][k] = c[size * gi + 0];
            gi++;
        }
    }

    // ------------------------ CUDA 6. Очистка всех видов памяти ------------------------
    // TODO

    free(c);
    free(A);
}

int main () {
    for (int k = 0; k <= K_MAX; k++) {
        count_state(k);
        print_state(k);
    }  

    return 0;
}

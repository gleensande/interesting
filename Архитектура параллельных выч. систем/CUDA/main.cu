#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "cuda.hpp"
#include "cuda_runtime.hpp"

// матрица состояний пластины
double p[I_MAX + 1][J_MAX + 1][K_MAX + 1];

void substract_rows_Gauss(int i_from, int i_to, double* A, int size, double* fr) {
    double coeff = A[size * i_to + i_from];
    for (int j = 0; j < size; j++) {
        A[size * i_to + j] -= A[size * i_from + j] * coeff;
    }
    fr[i_to] -= fr[i_from] * coeff;
}

// метод Гаусса
__global__ void Gauss(double* A, int size, double* fr) {
    double coeff;

    // forward
    for (int i = 0; i < size - 1; i++) {
        // > ---- make diag one ----
        coeff = A[i * size + i];
        for (int j = 0; j < size; j++) {
            A[i * size + j] /= coeff;
        }
        fr[i] /= coeff;
        // < ---- make diag one ----

        for (int k = i + 1; k < size; k++) {            
            // > ---- substract rows ----
            double coeff = A[size * k + i];
            for (int j = 0; j < size; j++) {
                A[size * k + j] -= A[size * i + j] * coeff;
            }
            fr[k] -= fr[i] * coeff;
            // < ---- substract rows ----
        }
    }
    // > ---- make diag one ----
    coeff = A[(size - 1) * size + (size - 1)];
    for (int j = 0; j < size; j++) {
        A[(size - 1) * size + j] /= coeff;
    }
    fr[(size - 1)] /= coeff;
    // < ---- make diag one ----

    // backward
    for (int i = size - 1; i > 0 ; i--) {
        for (int k = i - 1; k >= 0; k--) {
            // > ---- substract rows ----
            double coeff = A[size * k + i];
            for (int j = 0; j < size; j++) {
                A[size * k + j] -= A[size * i + j] * coeff;
            }
            fr[k] -= fr[i] * coeff;
            // < ---- substract rows ----
        }
    }
}

// напечатать состояние
void print_state(int k) {
    printf("Матрица p:\n");
    for (int i = 0; i <= I_MAX; i++) {
        for (int j = 0; j <= J_MAX; j++) {
            printf("%8.3lf ", p[i][j][k]);
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

void print_matrix(double* A, int rows, int cols, char* name) {
    printf("%s\n", name);
    /*for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%5.2lf ", A[cols * i + j]);
        }
        printf("\n");
    }*/
    for (int i = 0; i < rows * cols; i++) {
        printf("%lf\n", A[i]);
    }
    printf("\n");
}

void prepare_state(double* C, double* A, int size, int cmax, int k) {
    // Задание граничных условий в матрице

    // верх и низ
    // граничное условие первого рода
    for (int m = 0; m <= J_MAX; m++) {
        A[m * size + m] = 1;
        C[m] = T_UP();

        A[(cmax - m) * size + (cmax - m)] = 1;
        C[cmax - m] = T_BOTTOM();
    }

    // левая граница - каждый J_MAX + 1 -ый элемент
    // граничное условие второго рода
    for (int m = 0; m <= cmax; m += J_MAX + 1) {
        A[m * size + m] = 1;
        A[m * size + (m + 1)] = -1;
        C[m] = 0;
    }

    // правая граница - каждый J_MAX -ый элемент
    // граничное условие первого рода
    for (int m = J_MAX; m <= cmax; m += J_MAX + 1) {
        A[m * size + m] = 1;
        C[m] = T_RIGHT();
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
            C[q+m] = p[(q+m)/(J_MAX+1)][(q+m)%(J_MAX+1)][k-1]*T_prev_coeff;
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

    double* C = (double*)malloc(sizeof(double) * size);
    double* A = (double*)malloc(sizeof(double) * size * size);

    prepare_state(C, A, size, cmax, k);

    // ----------------- CUDA 2. Выделение памяти девайса и заполнение ее -----------------
    double *dC, *dA, *dsize; 
    cudaMalloc((void**)(&dC), sizeof(double) * size);
    cudaMalloc((void**)(&dA), sizeof(double) * size * size);
    cudaMalloc((void**)(&dsize), sizeof(int));

    // --------------------- CUDA 3. Перенос данных из хоста девайсу ---------------------
    cudaMemcpy((void*)dC, (void*)C, sizeof(double) * size, cudaMemcpyToDevice);
    cudaMemcpy((void*)dA, (void*)A, sizeof(double) * size * size, cudaMemcpyToDevice);
    cudaMemcpy((void*)dsize, (void*)(&size)), sizeof(int));

    // ----------------------- CUDA 4. Вызов ядра для решения СЛАУ -----------------------
    Gauss<<<1,1>>>(dA, dsize, dC);
    
    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();


    // --------------------- CUDA 5. Перенос данных из девайса хосту ---------------------
    cudaMemcpy((void*)C, (void*)dC, sizeof(double) * size, cudaMemcpyToHost);

    // перенос значений из вектора C в матрицу p
    int gi = 0;
    for (int i = 0; i < I_MAX + 1; i++) {
        for (int j = 0; j < J_MAX + 1; j++) {
            p[i][j][k] = C[gi];
            gi++;
        }
    }

    // ------------------------ CUDA 6. Очистка всех видов памяти ------------------------
    cudaFree(dC);
    cudaFree(dA);
    cudaFree(dsize);

    free(C);
    free(A);
}

int main () {
    for (int k = 0; k <= K_MAX; k++) {
        count_state(k);
        print_state(k);
    }  

    return 0;
}

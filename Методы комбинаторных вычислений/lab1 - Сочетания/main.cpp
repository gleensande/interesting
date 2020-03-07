#include "matrix.hpp"

#define M 3
#define N 5

using namespace std;

void print_combination(int C[M]) {
    for (int i = 0; i < M; i++) {
        cout << C[i];
    }
    cout << endl;
}

void array_to_Matrix(double* arr, int a, int b, Matrix<double>* m) {
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
	   		m->set(i, j, arr[i*b + j]);
		}
	}
}

void solve_slay(int basis[M]) {
	// задание начальных значений для СЛАУ
	Matrix<double> A(N - 2, N);
	double a[N][N] = {
			{1, 1, 1, 0, 0},
			{-1, 1, 0, 1, 0},
			{2, 1, 0, 0, 1},
	};
	array_to_Matrix(*a, N - 2, N, &A);

	Matrix<double> FR(N - 2, 1);
	double fr[N][1] = {
			{4},
			{2},
			{6},
	};	
	array_to_Matrix(*fr, N - 2, 1, &FR);

	// поиск свободных переменных
	int non_basis[N-M];

	// переменные, которые присутствуют в базисе
	// будут отмечены по индексу i-1 значением true 
	bool exists_in_basis[N];
	
	// false по всему массиву
	for (int i = 0; i < N; i++) {
		exists_in_basis[i] = false;
	}

	// отмечаем присутствие
	for (int i = 0; i < M; i++) {
		exists_in_basis[basis[i] - 1] = true;	
	}

	// выделяем отсутствующие номера
	int j = 0;
	for (int i = 0; i < N; i++) {
		if (exists_in_basis[i] == false) {
			non_basis[j] = i;
			j++;
		}
	}

	// удаление столбцов свободных переменных,
	// чтобы упростить решение методом Гаусса
	// A.print(&FR, "Начальное уравнение:");

	// удаление всех столбцов, соответствующих
	// свободным переменным
	A.delete_cols(non_basis, N-M);

	// печать полученного уравнения
	// A.print(&FR, "После удаления столбцов свободных переменных:");

	// решение слау
	A.Gauss(&FR);

	// печать решения
	// A.print(&FR, "Ответ:");

	// печать базисных переменных (по ним упорядочено)
	for (int i = 0; i < M; i++) {
		cout << "X" << basis[i] << " = " << FR.get(i, 0) << ", ";
	}

	for (int i = 0; i < N - M; i++) {
		cout << "X" << non_basis[i] + 1 << ", ";
	}
	cout << "\b\b = 0\n";
}

int main() {
	// задание начальных значений сочетаний
	int Cmax[M], C[M];
	for (int i = 0; i < M; i++) {
        Cmax[i] = N + i - M + 1;
    }

    for (int i = 0; i < M; i++) {
        C[i] = i + 1;
    }

	// алгоритм перечисления + решение слау на каждой итерации
	// print_combination(C);
	solve_slay(C);

	while (C[0] != Cmax[0]) {
        // поиск справа-налево самого правого элемента,
		// не достигшего своего допустимого максимума
        int i_change = 0;
        for (int i = M - 1; (i >= 0) && (i_change == 0); i--) {
            if(C[i] < Cmax[i]) {
               i_change = i;
            }
        }

        // увеличение следующих за ним элементов
        C[i_change]++;
        for (int j = i_change + 1; j < M; j++) {
            C[j] = C[i_change] + j - i_change;
        }

		// печать нового сочетания и решение слау с ним
		// print_combination(C);
		solve_slay(C);
	}

	return 0;
}

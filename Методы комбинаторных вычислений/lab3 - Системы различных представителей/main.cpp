#include "matrix.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

#define rows 6
#define cols 6

using namespace std;


// проверяет, есть ли число x в векторе v
bool is_in_vector(int x, vector<int>& v) {
    if (find(v.begin(), v.end(), x) == v.end()) {
        return false;
    }
    
    return true;
}


// возвращает индекс элемента x в векторе v
// или -1, если такого элемента в векторе нет
int index_in_vector(int x, vector<int>& v) {
    auto it = find(v.begin(), v.end(), x);

    if (it == v.end()) {
        return -1; 
    }

    return distance(v.begin(), it);
}


// копирует вектор original и добавляет конец нового вектора
// все элементы из additional, которых там изначально не было
vector<int> sum_vectors(vector<int> original, vector<int>& additional) {
    vector<int> sum = original;
    for (int a : additional) {
        if (!is_in_vector(a, original)) {
            sum.push_back(a);
        }
    }

    return sum;
}


// печатает вектор в {...} и с его именем вначале
void print_vector(vector<int>& v, string name) {
    cout << name << " = { ";
    int i;
    for (i = 0; i < v.size() - 1; i++) {
        cout << v[i] << ", ";
    }
    cout << v[i] << " }" << endl;
}


// создает исходную матрицу из заданного массива
Matrix<int> create_input_matrix() {
    ifstream matrix_file("matrix.txt");

    if (!matrix_file.is_open()) {
        cout << "Error, can't open matrix file" << endl;
    }

    int drows, dcols;

    matrix_file >> drows;
    
    cout << "drows" << " = " << drows << endl;

    int input_matrix[rows][cols] = {
        {0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0},
        {1, 1, 0, 0, 1, 0},
        {0, 0, 0, 1, 0, 0},
        {1, 0, 0, 1, 1, 0},
        {0, 1, 0, 0, 0, 0},
    };

    Matrix<int> A(*input_matrix, rows, cols);
    A.print("Input matrix:");

    return A;
}


// превращает матрицу в вектор семейств позиций единиц
vector< vector<int> > turn_to_families(Matrix<int>& A) {
    vector< vector<int> > S;

    for (int i = 0; i < A.get_rows(); i++) {
        vector<int> current_family;
        for (int j = 0; j < A.get_cols(); j++) {
            if (A.get(i, j) == 1) {
                current_family.push_back(j + 1);
            }
        }
        S.push_back(current_family);
    }

    cout << "Families:\n";
    for (int i = 0; i < S.size(); i++) {
        print_vector(S[i], "S_" + to_string(i));
    }
    cout << endl;

    return S;
}


// создает начальную трансверсаль Т_0
vector<int> create_T_0(vector <vector<int> >& S) {
    vector<int> T_0;

    bool cant_set_next = false;
    for (int i = 0; i < S.size() && !cant_set_next; i++) {
        int current_agent_i = 0;
        while(!cant_set_next && is_in_vector(S[i][current_agent_i], T_0)) {
            current_agent_i++;
            if(current_agent_i >= S[i].size()) {
                cant_set_next = true;
            }
        }
        if (!cant_set_next) {
            T_0.push_back(S[i][current_agent_i]);
        }
    }

    cout << "First transversal: \n";
    print_vector(T_0, "T_0");
    cout << endl;

    return T_0;
}


// находит семейство из тех, которые мы прибаляли к L, в котором впервые встречается элемент x
// поиск только после индекса after, возврат -1, если такого элемента нет вообще
int index_in_add_families(map<int, vector<int> >& add_families, int x, int after = -1) {
    int i = 0;
    for (auto it = add_families.begin(); it != add_families.end(); it++) {
        i = it->first;
        if (i > after) {
            if (is_in_vector(x, add_families[i])) {
                return i;
            }
        }
    }
    return -1;
}


// находит представителя для следующего, еще не представленного, семейства
// возвращает новую трансверсаль
vector<int> find_next_agent(vector<int>& T, vector< vector<int> >& S) {
    int next_agent_i = T.size();

    cout << endl << "1. Viewing and forming of L-list:" << endl;

    vector<int> L = S[next_agent_i];         // L_0 = S[следующий, еще не представленный в T, номер]
    print_vector(L, "L_0");
    
    int j = 0;
    int p = index_in_vector(L[j], T);
    map<int, vector<int> > add_families;     // семейства, которые мы прибавляли к L
    while (p != -1) {
        add_families[p] = S[p];
        L = sum_vectors(L, S[p]);
        print_vector(L, "L_" + to_string(j + 1));

        j++;
        p = index_in_vector(L[j], T);
    }
    cout << L[j] << " ∉ T" << endl; 
    cout << endl;

    cout << "2. Replacement of agent:" << endl;
    vector<int> T_new = T;


    int agent = L[j], agent_swap = 0;
    int replace_i = -1;
    while (!is_in_vector(agent, S[next_agent_i])) {
        replace_i = index_in_add_families(add_families, agent, replace_i);
        agent_swap = T_new[replace_i];
        T_new[replace_i] = agent;

        cout << "{ " << agent << " } + T - { " << agent_swap << " }" << endl;

        agent = agent_swap;

        print_vector(T_new, "T");
        cout << endl;
    }

    cout << "{ " << agent << " } + T" << endl;
    T_new.push_back(agent);

    print_vector(T_new, "T");
    cout << endl;
 
    return T_new;
}


int main() {
    Matrix<int> A = create_input_matrix();
    /*vector< vector<int> > S = turn_to_families(A); 
    vector<int> T = create_T_0(S);

    for (int i = 1; T.size() != rows; i++) {
        cout << "Iteration #" << i << ':' << endl;;
        T = find_next_agent(T, S);
    }

    A.print("Result:", &T);*/

    return 0;
}
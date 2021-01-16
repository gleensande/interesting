#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "neuron.hpp"

using namespace std;

// Убирает пробелы с концов переданной строки
string trim_spaces(string line) {
    string trimmed;
    string space_like_characters = " \t\n";

    size_t first_non_space = line.find_first_not_of(space_like_characters);
    size_t last_non_space = line.find_last_not_of(space_like_characters);

    trimmed = line.substr(first_non_space, last_non_space - first_non_space + 1);

    return trimmed;
}

// Разделяет строку на массив строк по переданному разделителю, сокращая пробелы на концах разделенных строк
vector<double> csv_parser(string line, string delimiter) {
    vector<double> splited_line;

    size_t prev_pos = 0, delimiter_pos = line.find(delimiter);
    string sub_line;
    while (delimiter_pos != string::npos) {
        sub_line = line.substr(prev_pos, delimiter_pos - prev_pos);
        splited_line.push_back(stod(trim_spaces(sub_line)));

        prev_pos = delimiter_pos + delimiter.size();
        delimiter_pos = line.find(delimiter, prev_pos);
    }
    splited_line.push_back(stod(trim_spaces(line.substr(prev_pos))));

    return splited_line;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    if (argc != 2) {
        cout << "Usage: ./a.out coef" << endl;
        return 1;
    }
    double coef = stod(argv[1]);

    // чтение из файла обучающей выборки класса 0
    ifstream learn_data_file_0;
    learn_data_file_0.open("learn_data_0.dat");
    if (!learn_data_file_0) {
        cout << "ERROR: can't open file 'learn_data_0.dat'" << endl;
        return 1;
    }

    // чтение из файла обучающей выборки класса 1
    ifstream learn_data_file_1;
    learn_data_file_1.open("learn_data_1.dat");
    if (!learn_data_file_1) {
        cout << "ERROR: can't open file 'learn_data_0.dat'" << endl;
        return 1;
    }


    vector<double> x_local(2);
    string line;

    // получение отдельных данных из 0 файла
    vector<bool> D_learn_0;
    vector< vector<double> > X_learn_0;
    while(getline(learn_data_file_0, line)) {
        vector<double> values = csv_parser(line, " ");
        x_local[0] = values[0];
        x_local[1] = values[1];
        X_learn_0.push_back(x_local);
        D_learn_0.push_back(values[2]);
    }
    learn_data_file_0.close();

    // получение отдельных данных из 1 файла
    vector<bool> D_learn_1;
    vector< vector<double> > X_learn_1;
    while(getline(learn_data_file_1, line)) {
        vector<double> values = csv_parser(line, " ");
        x_local[0] = values[0];
        x_local[1] = values[1];
        X_learn_1.push_back(x_local);
        D_learn_1.push_back(values[2]);
    }
    learn_data_file_1.close();

    // формирование случайной выборки
    int size = D_learn_0.size() + D_learn_1.size();
    vector<bool> D_learn;
    vector< vector<double> > X_learn;
    int r;
    while(!(X_learn_0.size() == 0 && X_learn_1.size() == 0)) {
        r = random() % 2;

        if (r == 0) {
            if (X_learn_0.size() != 0) {
                r = random() % X_learn_0.size();
                X_learn.push_back(X_learn_0[r]);
                D_learn.push_back(0);
                X_learn_0.erase(X_learn_0.begin() + r);
            }
        } else {
            if (X_learn_1.size() != 0) {
                r = random() % X_learn_1.size();
                X_learn.push_back(X_learn_1[r]);
                D_learn.push_back(1);
                X_learn_1.erase(X_learn_1.begin() + r);
            }
        }
    }

    cout << "Обучающая выборка: " << endl;
    for (int i = 0; i < size; i++) {
        cout << X_learn[i][0] << " " << X_learn[i][1] << " " << D_learn[i] << endl;
    }
    cout << endl;

    cout << "Learning:" << endl;
    // создание и обучение персептрона    
    Perceptron P(2);
    P.learn(X_learn, D_learn, 100, coef);

    int c = 0;
    coef -= 0.1;
    while ((P.test(X_learn, D_learn) == false) && (c <= 10)) {
        P.learn(X_learn, D_learn, 100, coef);
        c++;
        coef -= 0.1;
    }

    // чтение из файла тестовой выборки
    ifstream test_data_file;
    test_data_file.open("test_data.dat");
    if (!test_data_file) {
        cout << "ERROR: can't open file 'test_data.dat'" << endl;
        return 1;
    }

    vector<bool> D_test;
    vector< vector<double> > X_test;
    while(getline(test_data_file, line)) {
        vector<double> values = csv_parser(line, " ");
        x_local[0] = values[0];
        x_local[1] = values[1];
        X_test.push_back(x_local);
        D_test.push_back(values[2]);
    }
    
    P.test(X_test, D_test);
    
    test_data_file.close();

    return 0;
}

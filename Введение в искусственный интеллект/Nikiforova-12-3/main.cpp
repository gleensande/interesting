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

int main(/*int argc, char** argv*/) {
    /*if (argc != 2) {
        cout << "Usage: ./a.out coef" << endl;
        return 1;
    }*/

    // чтение из файла обучающей выборки
    ifstream learn_data_file;
    learn_data_file.open("learn_data.dat");
    if (!learn_data_file) {
        cout << "ERROR: can't open file 'learn_data.dat'" << endl;
        return 1;
    }

    vector<bool> D_learn;
    vector< vector<double> > X_learn;
    vector<double> x_local(2);
    string line;
    while(getline(learn_data_file, line)) {
        vector<double> values = csv_parser(line, " ");
        x_local[0] = values[0];
        x_local[1] = values[1];
        X_learn.push_back(x_local);
        D_learn.push_back(values[2]);
    }

    learn_data_file.close();

    cout << "Learning:" << endl;
    // создание и обучение персептрона    
    Perceptron P(2);
    P.learn(X_learn, D_learn, 100, 1/*stod(argv[1])*/);

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
    
    cout << "Testing:" << endl;
    // тестирование нейрона
    for (size_t i = 0; i < D_test.size(); i++) {
        bool result = P.activation_func(X_test[i]);
        cout << "Neuron res = " << result << ", ";
        cout << "D = " << D_test[i] << ". ";
        if (result == D_test[i]) {
            cout << "Test passed!";
        } else {
            cout << "Test not passed!";
        }
        cout << endl;
    }
    
    learn_data_file.close();
    test_data_file.close();

    return 0;
}

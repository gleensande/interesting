#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "network.hpp"

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
    if (argc != 2) {
        cout << "Usage: ./a.out N" << endl;
        return 1;
    }
    int neurons_num = atoi(argv[1]);

    // чтение из файла обучающей выборки
    ifstream learn_data_file;
    learn_data_file.open("coordinates.dat");
    if (!learn_data_file) {
        cout << "ERROR: can't open file 'coordinates.dat'" << endl;
        return 1;
    }

    vector< vector<double> > X_learn;
    vector<double> x_local(2);
    string line;
    while(getline(learn_data_file, line)) {
        vector<double> values = csv_parser(line, " ");
        x_local[0] = values[0];
        x_local[1] = values[1];
        X_learn.push_back(x_local);
    }
    learn_data_file.close();
    
    Network N(neurons_num);
    N.learn(X_learn);
    
    return 0;
}

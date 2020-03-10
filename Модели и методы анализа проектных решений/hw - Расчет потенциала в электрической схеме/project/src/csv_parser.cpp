#include "csv_parser.hpp"

// Разделяет строку на массив строк по переданному разделителю,
// сокращая пробелы на концах разделенных строк
vector<string> csv_parser(string line, string delimiter) {
    vector<string> splited_line;

    size_t prev_pos = 0, delimiter_pos = line.find(delimiter);
    string sub_line;
    while (delimiter_pos != string::npos) {
        sub_line = line.substr(prev_pos, delimiter_pos - prev_pos);
        splited_line.push_back(trim_spaces(sub_line));

        prev_pos = delimiter_pos + delimiter.size();
        delimiter_pos = line.find(delimiter, prev_pos);
    }
    splited_line.push_back(trim_spaces(line.substr(prev_pos)));

    return splited_line;
}

// Убирает пробелы с концов переданной строки
string trim_spaces(string line) {
    string trimmed;
    string space_like_characters = " \t\n";

    size_t first_non_space = line.find_first_not_of(space_like_characters);
    size_t last_non_space = line.find_last_not_of(space_like_characters);

    trimmed = line.substr(first_non_space, last_non_space - first_non_space + 1);

    return trimmed;
}

// создание словаря ключ-значение из файла
map<string, double> create_map_from_file(string filename) {
    map<string, double> constants;

    // Открытие файла с константами
    ifstream data_file;
    data_file.open(filename);
    if (!data_file) {
        cout << "ОШИБКА: Невозможно открыть файл c данными : " << filename << endl;
        return constants;
    }


    // заполнение словаря константами
    string line;
    while(getline(data_file, line)) {
        if (line == "") {
            continue;
        }
        
        vector<string> name_and_value = csv_parser(line, "=");
        string name = name_and_value[0];
        double value = stod(name_and_value[1]);

        constants[name] = value;
    }

    return constants;
}
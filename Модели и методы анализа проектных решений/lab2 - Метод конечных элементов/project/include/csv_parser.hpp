#ifndef GCDCHM_CHEM_CSV_PARSER_HPP
#define GCDCHM_CHEM_CSV_PARSER_HPP

#include "std_include.hpp"

vector<string> csv_parser(string line, string delimiter = ";");
string trim_spaces(string line);
map<string, double> create_map_from_file(string filename);


#endif  // GCDCHM_CHEM_CSV_PARSER_HPP

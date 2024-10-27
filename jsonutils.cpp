//
// Created by rjrob on 25/10/2024.
//
#include <fstream>
#include "json.hpp"
using namespace nlohmann;
using namespace std;

json readJsonFile(string filename) {
    ifstream i(filename);
    json j;
    i >> j;

    return j;
}

void writeJsonFile(string filename, json j) {
    ofstream o(filename);
    o << std::setw(4) << j << std::endl;
}

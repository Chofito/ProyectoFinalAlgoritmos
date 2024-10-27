//
// Created by rjrob on 25/10/2024.
//

#ifndef JSONUTILS_H
#define JSONUTILS_H

#include "json.hpp"
using namespace nlohmann;
using namespace std;

json readJsonFile(string filename);
void writeJsonFile(string filename, json j);

#endif //JSONUTILS_H

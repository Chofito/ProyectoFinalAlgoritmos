//
// Created by rjrob on 25/10/2024.
//

#ifndef USERPLANS_H
#define USERPLANS_H

#include <iostream>
#include "json.hpp"
using namespace std;
using namespace nlohmann;

void addUserPlan(string username, string plan);
json getUserPlans(string username);

#endif //USERPLANS_H

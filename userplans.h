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
void deleteUserPlan(string username, string plan);
void deletePlanByUserEmail(string username);
void deletePlanByPlanName(string planName);
bool checkIfUserHasPlan(string username, string plan);
json getUserPlans(string username);

#endif //USERPLANS_H

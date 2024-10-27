//
// Created by rjrob on 25/10/2024.
//

#ifndef PLANS_H
#define PLANS_H

#include "json.hpp"
using namespace nlohmann;
using namespace std;

void addPlan(string name, string description, double price, string userObjective);
void editPlan(string name, string description, double price, string userObjective);
void deletePlan(string name);
bool checkIfPlanExists(string name);
json getPlan(string name);
json listPlans();
json listEnterprisePlans();
json listIndividualPlans();

#endif //PLANS_H

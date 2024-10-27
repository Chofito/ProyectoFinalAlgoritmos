//
// Created by rjrob on 25/10/2024.
//

#ifndef MAIN_H
#define MAIN_H

#include "json.hpp"
using namespace nlohmann;

int showGuestMenu(json user);
int showCostumerMenu(json user);
int showEmployeeMenu(json user);
int showAdminMenu(json user);

#endif //MAIN_H

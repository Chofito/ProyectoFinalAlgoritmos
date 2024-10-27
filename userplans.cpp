//
// Created by rjrob on 25/10/2024.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include "userplans.h"
#include "json.hpp"
#include "jsonutils.h"
using namespace nlohmann;
using namespace std;

void addUserPlan(string userEmail, string plan) {
    string filename = "userplans.json";

    json userplans = readJsonFile(filename);

    std::time_t now = std::time(nullptr);
    std::tm* tmnow = std::localtime(&now);
    tmnow->tm_mon += 1;

    std::time_t future_time = std::mktime(tmnow);

    userplans["userPlans"].push_back({
        {"userEmail", userEmail},
        {"plan", plan},
        {"timestamp", future_time}
    });

    writeJsonFile(filename, userplans);
}

json getUserPlans(string userEmail) {
    string filename = "userplans.json";

    json userPlans = readJsonFile(filename);

    json userPlansFiltered = json::parse(R"(
      {
        "userPlans": []
      }
    )");

    for (auto& item : userPlans["userPlans"]) {
        if (item["userEmail"].get<string>() == userEmail) {
            userPlansFiltered["userPlans"].push_back(item);
        }
    }

    return userPlansFiltered["userPlans"];
}

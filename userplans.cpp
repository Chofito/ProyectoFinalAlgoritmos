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

void deleteUserPlan(string userEmail, string plan) {
    string filename = "userplans.json";

    json userplans = readJsonFile(filename);

    for (auto it = userplans["userPlans"].begin(); it != userplans["userPlans"].end(); ++it) {
        if (it.value()["userEmail"].get<string>() == userEmail && it.value()["plan"].get<string>() == plan) {
            userplans["userPlans"].erase(it);
        }
    }

    writeJsonFile(filename, userplans);
}

void deletePlanByUserEmail(string userEmail) {
    string filename = "userplans.json";

    json userplans = readJsonFile(filename);

    for (auto it = userplans["userPlans"].begin(); it != userplans["userPlans"].end(); ++it) {
        if ((*it)["userEmail"].get<string>() == userEmail) {
            it = userplans["userPlans"].erase(it);
        }
    }

    writeJsonFile(filename, userplans);
}

void deletePlanByPlanName(string planName) {
    string filename = "userplans.json";

    json userplans = readJsonFile(filename);

    for (auto it = userplans["userPlans"].begin(); it != userplans["userPlans"].end(); ++it) {
        if ((*it)["plan"].get<string>() == planName) {
            it = userplans["userPlans"].erase(it);
        }
    }

    writeJsonFile(filename, userplans);
}

bool checkIfUserHasPlan(string userEmail, string planName) {
    string filename = "userplans.json";

    json userplans = readJsonFile(filename);

    for (auto& item : userplans["userPlans"]) {
        if (item["userEmail"].get<string>() == userEmail && item["plan"].get<string>() == planName) {
            return true;
        }
    }

    return false;
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

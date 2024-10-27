//
// Created by rjrob on 25/10/2024.
//
#include <iostream>
#include <fstream>
#include "plans.h"
#include "json.hpp"
#include "jsonutils.h"
using namespace std;
using namespace nlohmann;

void addPlan(string name, string description, double price, string userObjective) {
    string filename = "plans.json";

    json plansFileContent = readJsonFile(filename);

    plansFileContent["plans"].push_back({
        {"name", name},
        {"description", description},
        {"price", price},
        {"userObjective", userObjective}
    });

    writeJsonFile(filename, plansFileContent);
}

void editPlan(string name, string description, double price, string userObjective) {
    string filename = "plans.json";

    json plans = readJsonFile(filename);

    // Search for the item in the array based on name
    for (auto& item : plans["plans"]) {
        if (item["name"].get<string>() == name) {
            item["description"] = description.empty() ? item["description"].get<string>() : description;
            item["price"] = price == 0 ? item["price"].get<double>() : price;
            item["userObjective"] = userObjective.empty() ? item["userObjective"].get<string>() : userObjective;

            break;
        }
    }

    writeJsonFile(filename, plans);
}

void deletePlan(string name) {
    string filename = "plans.json";

    json plans = readJsonFile(filename);

    // Search for the item in the array based on name
    for (auto it = plans["plans"].begin(); it != plans["plans"].end(); ++it) {
        if (it.value()["name"].get<string>() == name) {
            it = plans["plans"].erase(it);
            break;
        }
    }

    writeJsonFile(filename, plans);
}

bool checkIfPlanExists(string name) {
    string filename = "plans.json";

    json plans = readJsonFile(filename);

    for (auto& item : plans["plans"]) {
        if (item["name"].get<string>() == name) {
            return true;
        }
    }

    return false;
}

json getPlan(string name) {
    string filename = "plans.json";

    json plans = readJsonFile(filename);

    for (auto& item : plans["plans"]) {
        if (item["name"].get<string>() == name) {
            return item;
        }
    }

    return json::parse(R"(
      {
        "isEmpty": true
    )");
}

json listPlans() {
    string filename = "plans.json";

    json plans = readJsonFile(filename);

    return plans["plans"];
}

json listEnterprisePlans() {
    string filename = "plans.json";

    json plans = readJsonFile(filename);

    json enterprisePlans = json::parse(R"(
      {
        "plans": []
      }
    )");

    for (auto& item : plans["plans"]) {
        if (item["userObjective"].get<string>() == "enterprise") {
            enterprisePlans["plans"].push_back(item);
        }
    }

    return enterprisePlans["plans"];
}

json listIndividualPlans() {
    string filename = "plans.json";

    json plans = readJsonFile(filename);

    json individualPlans = json::parse(R"(
      {
        "plans": []
      }
    )");

    for (auto& item : plans["plans"]) {
        if (item["userObjective"].get<string>() == "individual") {
            individualPlans["plans"].push_back(item);
        }
    }

    return individualPlans["plans"];
}

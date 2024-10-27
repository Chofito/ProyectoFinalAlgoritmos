//
// Created by rjrob on 25/10/2024.
//
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "initialsetup.h"
using namespace nlohmann;
using namespace std;

void setupUserFile() {
    string filename = "clients.json";
    json defaultUser = json::parse(R"(
      {
        "name": "Admin",
        "address": "",
        "email": "admin@admin.com",
        "password": "123",
        "phone": "",
        "type": "admin"
      }
    )");

    json clientsFileContent = json::parse(R"(
      {
        "clients": []
      }
    )");

    clientsFileContent["clients"].push_back(defaultUser);

    ifstream inFile(filename);

    if (!inFile) {
        ofstream o(filename);
        o << defaultUser << std::endl;
    }
}

void setupPlanFile() {
    string filename = "plans.json";

    ifstream inFile(filename);

    json plansInitial = json::parse(R"(
      {
        "plans": [
          {
            "name": "Basic+",
            "description": "This plan is special for new users, includes 20gb of data and 100 minutes of calls, unlimited social media apps, unlimited Youtube, Netflix, Tiktok and Spotify.",
            "price": 200,
            "onlyForNew": true,
            "userObjective": "individual"
          }
        ]
      }
    )");

    if (!inFile) {
        ofstream o(filename);
        o << plansInitial << std::endl;
    }
}

void setupTicketFile() {
    string filename = "tickets.json";

    ifstream inFile(filename);

    if (!inFile) {
        ofstream o(filename);
        o << "{\"tickets\": []}" << std::endl;
    }
}

void setupUserPlansFile() {
    string filename = "userplans.json";

    ifstream inFile(filename);

    if (!inFile) {
        ofstream o(filename);
        o << "{\"userPlans\": []}" << std::endl;
    }
}

void setupSoftware() {
    setupUserFile();
    setupPlanFile();
    setupTicketFile();
    setupUserPlansFile();
}

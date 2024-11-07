//
// Created by rjrob on 25/10/2024.
//
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "users.h"
#include "jsonutils.h"
using namespace nlohmann;
using namespace std;

void addUser(string name, string address, string phone, string email, string password, string type, string nit, string customerType) {
    string filename = "clients.json";

    json clients = readJsonFile(filename);

    clients["clients"].push_back({
        {"name", name},
        {"address", address},
        {"phone", phone},
        {"email", email},
        {"NIT", nit},
        {"password", password},
        {"customerType", customerType},
        {"type", type},
        {"isNew", true}
    });

    writeJsonFile(filename, clients);
}

void addCostumerUser(string name, string address, string phone, string email, string password, string nit, string customerType) {
    addUser(name, address, phone, email, password, "normal", nit, customerType);
}

void addEmployeeUser(string name, string address, string phone, string email, string password, string nit) {
    addUser(name, address, phone, email, password, "employee", nit, "employee");
}

void editUser(string name, string address, string email, string phone, string password, string nit, string customerType, string type) {
    string filename = "clients.json";

    json clients = readJsonFile(filename);

    // Search for the item in the array based on email
    for (auto& item : clients["clients"]) {
        if (item["email"].get<string>() == email) {
            item["name"] = name.empty() ? item["name"].get<string>() : name;
            item["address"] = address.empty() ? item["address"].get<string>() : address;
            item["phone"] = phone.empty() ? item["phone"].get<string>() : phone;
            item["NIT"] = nit.empty() ? item["NIT"].get<string>() : nit;
            item["password"] = password.empty() ? item["password"].get<string>() : password;
            item["customerType"] = customerType.empty() ? item["customerType"].get<string>() : customerType;
            item["type"] = type.empty() ? item["type"].get<string>() : type;

            break;
        }
    }

    writeJsonFile(filename, clients);
}

void editCustomerUser(string name, string address, string email, string phone, string password, string nit, string customerType) {
    editUser(name, address, email, phone, password, nit, customerType, "normal");
}

void editEmployeeUser(string name, string address, string email, string phone, string password, string nit) {
    editUser(name, address, email, phone, password,  nit, "employee", "employee");
}

void editAdminUser(string name, string address, string email, string phone, string password, string nit) {
    editUser(name, address, email, phone, password, nit, "admin",  "admin");
}

json makeUserNotNew(string email) {
    string filename = "clients.json";
    json userUpdated = json::parse("{}");

    json clients = readJsonFile(filename);

    // Search for the item in the array based on email
    for (auto& item : clients["clients"]) {
        if (item["email"].get<string>() == email) {
            item["isNew"] = false;

            userUpdated = item;

            break;
        }
    }

    writeJsonFile(filename, clients);

    return userUpdated;
}

void deleteUser(string email) {
    string filename = "clients.json";

    json clients = readJsonFile(filename);

    // delete user based on email
    for (auto it = clients["clients"].begin(); it != clients["clients"].end(); ++it) {
        if ((*it)["email"].get<string>() == email) {
            clients["clients"].erase(it);
            break;
        }
    }

    writeJsonFile(filename, clients);
}

int loginUser(string email, string password, json &userRef) {
    string filename = "clients.json";

    json clients = readJsonFile(filename);

    bool found = false;
    json user;

    // Search for the item in the array
    for (const auto& item : clients["clients"]) {
        if (item["email"].get<string>() == email) {
            user = item;
            found = true;

            break;
        }
    }

    if (found && user["password"] == password) {
        userRef = user;
        return 1;
    }

    return 0;
}

bool checkIfUserExists(string email) {
    string filename = "clients.json";

    json clients = readJsonFile(filename);

    // Search for the item in the array
    for (const auto& item : clients["clients"]) {
        if (item["email"].get<string>() == email) {
            return true;
        }
    }

    return false;
}

json listUsers() {
    string filename = "clients.json";

    json users = readJsonFile(filename);

    users = users["clients"];

    return users;
}

//
// Created by rjrob on 25/10/2024.
//

#ifndef USERS_H
#define USERS_H

#include "json.hpp"
using namespace nlohmann;
using namespace std;

void addUser(string name, string address, string phone, string email, string password, string type, string nit, string customerType);
void addCostumerUser(string name, string address, string phone, string email, string password, string nit, string customerType);
void addEmployeeUser(string name, string address, string phone, string email, string password, string nit);
void editUser(string name, string address, string email, string phone, string password, string nit, string customerType, string type);
void editCustomerUser(string name, string address, string email, string phone, string password, string nit, string customerType);
void editEmployeeUser(string name, string address, string email, string phone, string password, string nit);
void editAdminUser(string name, string address, string email, string phone, string password, string nit);
void deleteUser(string email);
int loginUser(string email, string password, json &userRef);
json makeUserNotNew(string email);
bool checkIfUserExists(string email);
json listUsers();

#endif //USERS_H

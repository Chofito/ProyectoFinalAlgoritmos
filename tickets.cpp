//
// Created by rjrob on 25/10/2024.
//
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "tickets.h"
#include "jsonutils.h"
using namespace nlohmann;
using namespace std;

void addTicket(string issue, string description, string ticketStatus, string reportedBy) {
    string filename = "tickets.json";

    json tickets = readJsonFile(filename);

    tickets["tickets"].push_back({
        {"issue", issue},
        {"description", description},
        {"ticketStatus", ticketStatus},
        {"reportedBy", reportedBy}
    });

    writeJsonFile(filename, tickets);
}

void editTicket(string issue, string description, string ticketStatus, string reportedBy) {
    string filename = "tickets.json";

    json tickets = readJsonFile(filename);

    // Search for the item in the array based on issue
    for (auto& item : tickets["tickets"]) {
        if (item["issue"].get<string>() == issue) {
            item["description"] = description.empty() ? item["description"].get<string>() : description;
            item["ticketStatus"] = ticketStatus.empty() ? item["ticketStatus"].get<string>() : ticketStatus;
            item["reportedBy"] = reportedBy.empty() ? item["reportedBy"].get<string>() : reportedBy;

            break;
        }
    }

    writeJsonFile(filename, tickets);
}

void deleteTicket(string issue) {
    string filename = "tickets.json";

    json tickets = readJsonFile(filename);

    // Search for the item in the array based on issue
    for (auto it = tickets["tickets"].begin(); it != tickets["tickets"].end(); ++it) {
        if (it.value()["issue"].get<string>() == issue) {
            tickets["tickets"].erase(it);

            break;
        }
    }

    writeJsonFile(filename, tickets);
}

bool checkIfTicketExists(string issue) {
    string filename = "tickets.json";

    json plans = readJsonFile(filename);

    for (auto& item : plans["tickets"]) {
        if (item["issue"].get<string>() == issue) {
            return true;
        }
    }

    return false;
}

json listTickets() {
    string filename = "tickets.json";

    json tickets = readJsonFile(filename);

    tickets = tickets["tickets"];

    return tickets;
}

json getTicketsByUser(string email) {
    string filename = "tickets.json";

    json tickets = readJsonFile(filename);

    json ticketsFiltered = json::parse(R"(
      {
        "tickets": []
      }
    )");

    for (auto& item : tickets["tickets"]) {
        if (item["reportedBy"].get<string>() == email) {
            ticketsFiltered["tickets"].push_back(item);
        }
    }

    return ticketsFiltered["tickets"];
}

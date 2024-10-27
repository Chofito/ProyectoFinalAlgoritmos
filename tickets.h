//
// Created by rjrob on 25/10/2024.
//

#ifndef TICKETS_H
#define TICKETS_H

#include "json.hpp"
using namespace nlohmann;
using namespace std;

void addTicket(string issue, string description, string ticketStatus, string reportedBy);
void editTicket(string issue, string description, string ticketStatus, string reportedBy);
void deleteTicket(string issue);
bool checkIfTicketExists(string issue);
json listTickets();
json getTicketsByUser(string email);

#endif //TICKETS_H

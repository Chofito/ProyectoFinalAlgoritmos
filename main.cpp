#include <iostream>
#include "initialsetup.h"
#include "json.hpp"
#include "users.h"
#include "plans.h"
#include "tickets.h"
#include "userplans.h"
using namespace nlohmann;
using namespace std;

int showGuestMenu(json &user) {
    // show menu
    // 1. Login
    // 2. Register
    // 3. Exit
    string input;
    int option;

    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Exit" << endl;
    cout << "Choose an option: ";

    getline(cin, input);
    stringstream(input) >> option;

    if (option == 1) {
        string email;
        string password;

        cout << "Email: ";
        getline(cin, email);
        cout << "Password: ";
        getline(cin, password);

        int loginStatus = loginUser(email, password, user);

        if (loginStatus == 0) {
            cout << "Invalid email or password." << endl;
            return -1;
        }

        if (loginStatus == 1) {
            if (user["type"].get<string>() == "normal") {
                return 1;
            }

            if (user["type"].get<string>() == "employee") {
                return 2;
            }

            if (user["type"].get<string>() == "admin") {
                return 3;
            }
        }
    }
    else if (option == 2) {
        string name;
        string nit;
        string address;
        string phone;
        string email;
        string password;
        string customerType;
        bool isValidEmail = false;
        bool isValidCustomerType = false;

        cout << "Please fill the following fields to register." << endl;
        cout << "Name and surname: ";
        getline(cin, name);
        cout << "Email: ";
        getline(cin, email);
        cout << "NIT: ";
        getline(cin, nit);
        cout << "Address: ";
        getline(cin, address);
        cout << "Phone: ";
        getline(cin, phone);
        cout << "Customer type (enterprise or individual): ";
        getline(cin, customerType);
        cout << "Password: ";
        getline(cin, password);

        if (customerType != "enterprise" && customerType != "individual") {
            cout << "Invalid customer type." << endl;

            while (!isValidCustomerType) {
                cout << "Customer type (enterprise or individual): ";
                getline(cin, customerType);

                if (customerType == "enterprise" || customerType == "individual") {
                    isValidCustomerType = true;
                } else {
                    cout << "Invalid customer type." << endl;
                }
            }
        }


        if (name.empty() || address.empty() || phone.empty() || email.empty() || password.empty()) {
            cout << "All fields are required." << endl;
            return -1;
        }

        bool emailAlreadyExists = checkIfUserExists(email);

        if (emailAlreadyExists) {
            cout << "User with the email " << email << " already exists." << endl;

            while (!isValidEmail) {
                cout << "Email: ";
                getline(cin, email);

                if (!checkIfUserExists(email)) {
                    isValidEmail = true;
                } else {
                    cout << "User with the email " << email << " already exists." << endl;
                }
            }
        }

        addCostumerUser(name, address, phone, email, password, nit, customerType);
    }
    else if (option == 3) {
        return 0;
    }

    return -1;
}

int showCostumerMenu(json &user) {
    if (user["type"].get<string>() == "normal" && user["isNew"].get<bool>() && user["customerType"].get<string>() == "individual") {
        cout << "Congratulations, you are a new user! You have access to an special plan!" << endl;

        json newMemberPlan = getPlan("Basic+");
        cout << "Plan name: " << newMemberPlan["name"].get<string>() << endl;
        cout << "Plan description: " << newMemberPlan["description"].get<string>() << endl;
        cout << "Plan price: " << newMemberPlan["price"].get<double>() << endl;
        cout << "Do you want to buy this plan? This offer is limited for this first login!" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        string input;
        int option;

        getline(cin, input);
        stringstream(input) >> option;

        if (option == 1) {
            addUserPlan(user["email"].get<string>(), newMemberPlan["name"].get<string>());
        }

        user = makeUserNotNew(user["email"].get<string>());
    }

    cout << endl << "Welcome " << user["name"].get<string>() << ", select an option" << endl;
    cout << "1. My plans" << endl;
    cout << "2. My tickets" << endl;
    cout << "3. Check plans" << endl;
    cout << "4. Open tickets" << endl;
    cout << "5. Logout" << endl;
    cout << "6. Exit" << endl;
    string input;
    int option;

    cout << "Choose an option: ";
    getline(cin, input);
    stringstream(input) >> option;

    switch (option) {
        // List user plans
        case 1: {
            json plans = getUserPlans(user["email"].get<string>());
            int counter = 1;

            cout << "Plans:" << endl;

            if (plans.empty()) {
                cout << "No plans found." << endl;
                break;
            }

            for (auto& item : plans) {
                cout << counter << ". " << "Plan name: " << item["plan"].get<string>() << endl;
                counter++;
            }

            system("pause");

            break;
        }
        // List user tickets
        case 2: {
            json tickets = getTicketsByUser(user["email"].get<string>());
            int counter = 1;

            cout << "Tickets:" << endl;

            if (tickets.empty()) {
                cout << "No tickets found." << endl;
                break;
            }

            for (auto& item : tickets) {
                cout << counter << ". " << "Ticket Issue: " << item["issue"].get<string>()  << ", Status: " << item["ticketStatus"].get<string>() << endl;
                counter++;
            }

            system("pause");

            break;
        }
        // List available plans for individual users and allow to buy a plan or see more details
        case 3: {
            json plans;
            int counter = 1;

            if (user["customerType"].get<string>() == "enterprise") {
                plans = listEnterprisePlans();
            } else {
                plans = listIndividualPlans();
            }

            if (plans.empty()) {
                cout << "No plans are available for " << user["customerType"].get<string>() <<" users." << endl;
                break;
            }

            cout << "The available plans for individual users are:" << endl;

            for (auto& item : plans) {
                cout << counter << ". " << "Plan name: " << item["name"].get<string>() << ", Price: " << item["price"].get<double>() << endl;
                counter++;
            }

            cout << endl << "Select an option:" << endl;
            cout << "1. Buy a plan" << endl;
            cout << "2. Show more details" << endl;
            cout << "3. Exist this menu" << endl;
            getline(cin, input);
            stringstream(input) >> option;

            if (option == 1) {
                string planName;
                cout << "Enter the name of the plan you want to buy: ";
                getline(cin, planName);

                bool planExists = checkIfPlanExists(planName);
                bool userAlreadyHasPlan = checkIfUserHasPlan(user["email"].get<string>(), planName);

                if (!planExists) {
                    cout << "Plan with the name " << planName << " does not exist." << endl;
                    break;
                }

                if (userAlreadyHasPlan) {
                    cout << "You already have the plan " << planName << "." << endl;
                    break;
                }

                addUserPlan(user["email"].get<string>(), planName);
            } else if (option == 2) {
                string planName;
                cout << "Enter the name of the plan you want to see details: ";
                getline(cin, planName);

                bool planExists = checkIfPlanExists(planName);

                if (!planExists) {
                    cout << "Plan with the name " << planName << " does not exist." << endl;
                    break;
                }

                json plan = getPlan(planName);

                cout << "Plan name: " << plan["name"].get<string>() << endl;
                cout << "Plan description: " << plan["description"].get<string>() << endl;
                cout << "Plan price: " << plan["price"].get<double>() << endl;
            }

            break;
        }
        // Open a ticket
        case 4: {
            string issue;
            string description;

            cout << "Are you experiencing any issue? Please let us know." << endl;
            cout << "Issue: ";
            getline(cin, issue);
            cout << "Description: ";
            getline(cin, description);

            addTicket(issue, description, "open", user["email"].get<string>());

            cout << "Ticket created successfully." << endl;

            break;
        }
        // Logout
        case 5: {
            return -1;
        }
        // Exit with status code 0
        case 6: {
            return 0;
        }
        default: {
            cout << "Invalid option." << endl;
            break;
        }
    }

    return 1;
}

int showEmployeeMenu(json &user) {
    cout << "1. Get plans by user" << endl;
    cout << "2. Get tickets by user" << endl;
    cout << "3. Show plan details" << endl;
    cout << "4. Show ticket details" << endl;
    cout << "5. Add plan" << endl;
    cout << "6. Add ticket" << endl;
    cout << "7. Edit plan" << endl;
    cout << "8. Delete plan" << endl;
    cout << "9. List plans" << endl;
    cout << "10. List tickets" << endl;
    cout << "11. Logout" << endl;
    cout << "12. Exit" << endl;
    string input;
    int option;

    cout << "Choose an option: ";
    getline(cin, input);
    stringstream(input) >> option;

    switch (option) {
        // Get plans by user
        case 1: {
            string userEmail;

            cout << "Enter the user's email to validate their subscribed plans." << endl;
            cout << "User email: ";
            getline(cin, userEmail);

            bool isValidEmail = checkIfUserExists(userEmail);

            if (!isValidEmail) {
                cout << "User with the email " << userEmail << " does not exist." << endl;
                break;
            }

            json plans = getUserPlans(userEmail);

            cout << "Plans for user " << userEmail << ":" << endl;

            if (plans.empty()) {
                cout << "No plans found for user " << userEmail << "." << endl;
                break;
            }

            int index = 1;
            for (auto& item : plans) {
                cout << index << ". " << "Plan name: " << item["plan"].get<string>() << endl;
                index++;
            }

            cout << " " << endl;

            system("pause");

            break;
        }
        // Get tickets by user
        case 2: {
            string userEmail;

            cout << "Enter the user's email to validate their opened tickets." << endl;
            cout << "User email: ";
            getline(cin, userEmail);

            bool isValidEmail = checkIfUserExists(userEmail);

            if (!isValidEmail) {
                cout << "User with the email " << userEmail << " does not exist." << endl;
                break;
            }

            json tickets = getTicketsByUser(userEmail);

            if (tickets.empty()) {
                cout << "No tickets found for user " << userEmail << "." << endl;
                break;
            }

            cout << "Tickets for user " << userEmail << ":" << endl;

            int index = 1;
            for (auto& item : tickets) {
                cout << index << ". " << "Ticket Issue: " << item["issue"].get<string>()  << ", Status: " << item["ticketStatus"].get<string>() << endl;
                index++;
            }

            system("pause");

            break;
        }
        // Show plan details
        case 3: {
            string planName;

            cout << "Enter the name of the plan you want to see details: ";
            cout << "Plan name: ";
            getline(cin, planName);

            bool planExists = checkIfPlanExists(planName);

            if (!planExists) {
                cout << "Plan with the name " << planName << " does not exist." << endl;
                break;
            }

            json plan = getPlan(planName);

            cout << "|----------- Plan details -----------|" << endl;
            cout << "Plan name: " << plan["name"].get<string>() << endl;
            cout << "Plan description: " << plan["description"].get<string>() << endl;
            cout << "Plan price: " << plan["price"].get<double>() << endl;
            cout << "User objective: " << plan["userObjective"].get<string>() << endl;

            system("pause");

            break;
        }
        // Show ticket details
        case 4: {
            string ticketIssue;

            cout << "Enter the ticket name to see details." << endl;
            cout << "Ticket issue name: ";
            getline(cin, ticketIssue);

            bool ticketExists = checkIfTicketExists(ticketIssue);

            if (!ticketExists) {
                cout << "Ticket with the issue " << ticketIssue << " does not exist." << endl;
                break;
            }

            json ticketDetails = getTicket(ticketIssue);

            if (ticketDetails["issue"].get<string>() == "not found") {
                cout << "Ticket with the issue " << ticketIssue << " does not exist." << endl;
                break;
            }

            cout << "|----------- Ticket details -----------|" << endl;
            cout << "Ticket issue: " << ticketDetails["issue"].get<string>() << endl;
            cout << "Ticket description: " << ticketDetails["description"].get<string>() << endl;
            cout << "Ticket status: " << ticketDetails["ticketStatus"].get<string>() << endl;
            cout << "Ticket reported by: " << ticketDetails["reportedBy"].get<string>() << endl;

            system("pause");

            break;
        }
        // Add plan
        case 5: {
            string planName;
            string planDescription;
            double planPrice;
            string userObjective;

            cout << "Plan name: ";
            getline(cin, planName);
            cout << "Plan description: ";
            getline(cin, planDescription);
            cout << "Plan price: ";
            getline(cin, input);
            stringstream(input) >> planPrice;
            cout << "Please define if this plan is for Enterprise or Individual users: ";
            getline(cin, userObjective);

            if (planName.empty() || planDescription.empty() || planPrice == 0) {
                cout << "All fields are required." << endl;
                break;
            }

            if (planPrice < 0) {
                cout << "Price must be greater than 0." << endl;
                break;
            }

            if (userObjective != "enterprise" && userObjective != "individual") {
                cout << "Invalid user objective." << endl;
                break;
            }

            addPlan(planName, planDescription, planPrice, userObjective);

            break;
        }
        // Add ticket
        case 6: {
            string ticketTitle;
            string ticketDescription;
            string ticketStatus;
            string ticketUser;

            cout << "Ticket title: ";
            getline(cin, ticketTitle);
            cout << "Ticket description: ";
            getline(cin, ticketDescription);
            cout << "Ticket status: ";
            getline(cin, ticketStatus);
            cout << "Ticket user: ";
            ticketUser = user["email"].get<string>();

            if (ticketTitle.empty() || ticketDescription.empty() || ticketStatus.empty() || ticketUser.empty()) {
                cout << "All fields are required." << endl;
                break;
            }

            if (ticketStatus != "open" && ticketStatus != "closed") {
                cout << "Invalid ticket status." << endl;
                break;
            }

            addTicket(ticketTitle, ticketDescription, ticketStatus, ticketUser);

            break;
        }
        // Edit plan
        case 7: {
            string planName;
            string planDescription;
            double planPrice;
            string userObjective;

            cout << "Plan name: ";
            getline(cin, planName);
            cout << "Plan description: ";
            getline(cin, planDescription);
            cout << "Plan price: ";
            getline(cin, input);
            stringstream(input) >> planPrice;
            cout << "Please define if this plan is for Enterprise or Individual users: ";
            getline(cin, userObjective);

            if (planName.empty() || planDescription.empty() || planPrice == 0) {
                cout << "All fields are required." << endl;
                break;
            }

            if (planPrice > 0) {
                cout << "Price must be greater than 0." << endl;
                break;
            }

            bool planExists = checkIfPlanExists(planName);

            if (!planExists) {
                cout << "Plan with the name " << planName << " does not exist." << endl;
                break;
            }

            if (userObjective != "enterprise" && userObjective != "individual") {
                cout << "Invalid user objective." << endl;
                break;
            }

            editPlan(planName, planDescription, planPrice, userObjective);

            break;
        }
        // Delete plan
        case 8: {
            string planName;

            cout << "Plan name: ";
            getline(cin, planName);

            bool planExists = checkIfPlanExists(planName);

            if (!planExists) {
                cout << "Plan with the name " << planName << " does not exist." << endl;
                break;
            }

            deletePlan(planName);
            deletePlanByPlanName(planName);

            break;
        }
        // List plans
        case 9: {
            json plans = listPlans();
            int counter = 1;

            cout << "Plans:" << endl;

            for (auto& item : plans) {
                cout << counter << ". " << "Name: " << item["name"].get<string>()  << ", Price: " << item["price"].get<double>() << ", User objective: " << item["userObjective"].get<string>() << endl;
                counter++;
            }

            system("pause");

            break;
        }
        // List tickets
        case 10: {
            json tickets = listTickets();
            int counter = 1;

            cout << "Tickets:" << endl;

            for (auto& item : tickets) {
                cout << counter << ". " << "Ticket Issue: " << item["issue"].get<string>()  << ", Status: " << item["ticketStatus"].get<string>() << endl;
                counter++;
            }

            system("pause");

            break;
        }
        case 11: {
            return -1;
        }
        // Exit with status code 0
        case 12: {
            return 0;
        }
        default: {
            cout << "Invalid option." << endl;
            break;
        }
    }

    return 1;
}

int showAdminMenu(json &user) {
    cout << "1. Add user" << endl;
    cout << "2. Add plan" << endl;
    cout << "3. Add ticket" << endl;
    cout << "4. Edit user" << endl;
    cout << "5. Edit plan" << endl;
    cout << "6. Edit ticket" << endl;
    cout << "7. Delete user" << endl;
    cout << "8. Delete plan" << endl;
    cout << "9. Delete ticket" << endl;
    cout << "10. List users" << endl;
    cout << "11. List plans" << endl;
    cout << "12. List tickets" << endl;
    cout << "13. Logout" << endl;
    cout << "14. Exit" << endl;
    string input;
    int option;

    cout << "Choose an option: ";
    getline(cin, input);
    stringstream(input) >> option;

    switch (option) {
        // Add user
        case 1: {
            string name;
            string address;
            string phone;
            string email;
            string nit;
            string password;
            string type;
            string customerType;
            bool isValidType = false;
            bool isValidEmail = false;
            bool isValidCustomerType = false;

            cout << "Please fill the following fields to register a new user." << endl;
            cout << "Name: ";
            getline(cin, name);
            cout << "NIT: ";
            getline(cin, nit);
            cout << "Email: ";
            getline(cin, email);
            cout << "Address: ";
            getline(cin, address);
            cout << "Phone: ";
            getline(cin, phone);
            cout << "User Type between employee, normal (costumer) and admin: ";
            getline(cin, type);

            if (type == "normal") {
                cout << "Customer type (enterprise or individual): ";
                getline(cin, customerType);
            } else {
                customerType = "individual";
            }

            cout << "Password: ";
            getline(cin, password);


            if (type != "normal" && type != "employee" && type != "admin") {
                cout << "Invalid user type." << endl;

                while (!isValidType) {
                    cout << "Type (normal, employee or admin): ";
                    getline(cin, type);

                    if (type == "normal" || type == "employee" || type == "admin") {
                        isValidType = true;
                    } else {
                        cout << "Invalid user type." << endl;
                    }
                }
            }

            // check that all values are filled
            if (name.empty() || address.empty() || phone.empty() || email.empty() || password.empty() || type.empty()) {
                cout << "All fields are required." << endl;
                break;
            }

            int userExists = checkIfUserExists(email);

            if (userExists == 1) {
                cout << "User with the email " << email << " already exists." << endl;

                while (!isValidEmail) {
                    cout << "Email: ";
                    getline(cin, email);

                    if (checkIfUserExists(email) == 0) {
                        isValidEmail = true;
                    } else {
                        cout << "User with the email " << email << " already exists." << endl;
                    }
                }
            }

            if (customerType != "enterprise" && customerType != "individual") {
                cout << "Invalid customer type." << endl;

                while (!isValidCustomerType) {
                    cout << "Customer type (enterprise or individual): ";
                    getline(cin, customerType);

                    if (customerType == "enterprise" || customerType == "individual") {
                        isValidCustomerType = true;
                    } else {
                        cout << "Invalid customer type." << endl;
                    }
                }
            }

            if (type == "normal") {
                addCostumerUser(name, address, phone, email, password, nit, customerType);
            } else if (type == "employee") {
                addEmployeeUser(name, address, phone, email, password, nit);
            } else if (type == "admin") {
                addUser(name, address, phone, email, password, type, nit, customerType);
            }
            break;
        }
        // Add plan
        case 2: {
            string planName;
            string planDescription;
            double planPrice;
            string userObjective;

            cout << "Plan name: ";
            getline(cin, planName);
            cout << "Plan description: ";
            getline(cin, planDescription);
            cout << "Plan price: ";
            getline(cin, input);
            stringstream(input) >> planPrice;
            cout << "Please define if this plan is for Enterprise or Individual users: ";
            getline(cin, userObjective);

            if (planName.empty() || planDescription.empty() || planPrice == 0) {
                cout << "All fields are required." << endl;
                break;
            }

            if (planPrice < 0) {
                cout << "Price must be greater than 0." << endl;
                break;
            }

            if (userObjective != "enterprise" && userObjective != "individual") {
                cout << "Invalid user objective." << endl;
                break;
            }

            addPlan(planName, planDescription, planPrice, userObjective);

            break;
        }
        // Add ticket
        case 3: {
            string ticketTitle;
            string ticketDescription;
            string ticketStatus;
            string ticketUser;

            cout << "Ticket title: ";
            getline(cin, ticketTitle);
            cout << "Ticket description: ";
            getline(cin, ticketDescription);
            cout << "Ticket status: ";
            getline(cin, ticketStatus);
            cout << "Ticket user: ";
            getline(cin, ticketUser);

            if (ticketTitle.empty() || ticketDescription.empty() || ticketStatus.empty() || ticketUser.empty()) {
                cout << "All fields are required." << endl;
                break;
            }

            if (ticketStatus != "open" && ticketStatus != "closed") {
                cout << "Invalid ticket status." << endl;
                break;
            }

            bool userExists = checkIfUserExists(ticketUser);

            if (!userExists) {
                cout << "User with the email " << ticketUser << " does not exist." << endl;
                cout << "Tickets can only be created for existing users." << endl;

                system("pause");

                break;
            }

            addTicket(ticketTitle, ticketDescription, ticketStatus, ticketUser);

            break;
        }
        // Edit user
        case 4: {
            string newName;
            string newAddress;
            string newPhone;
            string newEmail;
            string newPassword;
            string newType;

            cout << "Email: ";
            getline(cin, newEmail);
            cout << "Name: ";
            getline(cin, newName);
            cout << "Address: ";
            getline(cin, newAddress);
            cout << "Phone: ";
            getline(cin, newPhone);
            cout << "Password: ";
            getline(cin, newPassword);
            cout << "Type: ";
            getline(cin, newType);

            if (newType != "normal" && newType != "employee" && newType != "admin") {
                cout << "Invalid user type." << endl;
                break;
            }

            // check that all values are filled
            if (newName.empty() || newAddress.empty() || newPhone.empty() || newEmail.empty() || newPassword.empty() || newType.empty()) {
                cout << "All fields are required." << endl;
                break;
            }

            bool userExists = checkIfUserExists(newEmail);

            if (!userExists) {
                cout << "User with the email " << newEmail << " does not exist." << endl;
                break;
            }

            break;
        }
        // Edit plan
        case 5: {
            string planName;
            string planDescription;
            double planPrice;
            string userObjective;

            cout << "Plan name: ";
            getline(cin, planName);
            cout << "Plan description: ";
            getline(cin, planDescription);
            cout << "Plan price: ";
            getline(cin, input);
            stringstream(input) >> planPrice;
            cout << "Please define if this plan is for Enterprise or Individual users: ";
            getline(cin, userObjective);

            if (planName.empty() || planDescription.empty() || planPrice == 0) {
                cout << "All fields are required." << endl;
                break;
            }

            if (planPrice > 0) {
                cout << "Price must be greater than 0." << endl;
                break;
            }

            bool planExists = checkIfPlanExists(planName);

            if (!planExists) {
                cout << "Plan with the name " << planName << " does not exist." << endl;
                break;
            }

            if (userObjective != "enterprise" && userObjective != "individual") {
                cout << "Invalid user objective." << endl;
                break;
            }

            editPlan(planName, planDescription, planPrice, userObjective);

            break;
        }
        // Edit ticket
        case 6: {
            string ticketTitle;
            string ticketDescription;
            string ticketStatus;
            string ticketUser;

            cout << "Ticket title: ";
            getline(cin, ticketTitle);
            cout << "Ticket description: ";
            getline(cin, ticketDescription);
            cout << "Ticket status: ";
            getline(cin, ticketStatus);
            cout << "Ticket user: ";
            ticketUser = user["email"].get<string>();

            if (ticketTitle.empty() || ticketDescription.empty() || ticketStatus.empty() || ticketUser.empty()) {
                cout << "All fields are required." << endl;
                break;
            }

            if (ticketStatus != "open" && ticketStatus != "closed") {
                cout << "Invalid ticket status." << endl;
                break;
            }

            bool ticketExists = checkIfTicketExists(ticketTitle);

            if (!ticketExists) {
                cout << "Ticket with the title " << ticketTitle << " does not exist." << endl;
                break;
            }

            editTicket(ticketTitle, ticketDescription, ticketStatus, ticketUser);

            break;
        }
        // Delete user
        case 7: {
            string email;

            cout << "User Email to delete: ";
            getline(cin, email);

            bool userExists = checkIfUserExists(email);

            if (!userExists) {
                cout << "User with the email " << email << " does not exist." << endl;
                break;
            }

            deleteUser(email);
            // deletePlanByUserEmail(email);
            // deleteTicketsByUser(email);

            cout << "User with the email " << email << " has been deleted." << endl;
            cout << " " << endl;

            break;
        }
        // Delete plan
        case 8: {
            string planName;

            cout << "Plan name: ";
            getline(cin, planName);

            bool planExists = checkIfPlanExists(planName);

            if (!planExists) {
                cout << "Plan with the name " << planName << " does not exist." << endl;
                break;
            }

            deletePlan(planName);
            // deletePlanByPlanName(planName);

            break;
        }
        // Delete ticket
        case 9: {
            string issueName;

            cout << "Issue name: ";
            getline(cin, issueName);

            if (issueName.empty()) {
                cout << "All fields are required." << endl;
                break;
            }

            bool ticketExists = checkIfTicketExists(issueName);

            if (!ticketExists) {
                cout << "Ticket with the issue name " << issueName << " does not exist." << endl;
                break;
            }

            deleteTicket(issueName);

            break;
        }
        // List users
        case 10: {
            json users = listUsers();
            int counter = 1;

            cout << "Users:" << endl;

            for (auto& item : users) {
                cout << counter << ". " << "Name: " << item["name"].get<string>() << ", Email: " << item["email"].get<string>() << ", Type: " << item["type"].get<string>() << endl;
                counter++;
            }

            system("pause");

            break;
        }
        // List plans
        case 11: {
            json plans = listPlans();
            int counter = 1;

            cout << "Plans:" << endl;

            for (auto& item : plans) {
                cout << counter << ". " << "Name: " << item["name"].get<string>()  << ", Price: " << item["price"].get<double>() << ", User objective: " << item["userObjective"].get<string>() << endl;
                counter++;
            }

            system("pause");

            break;
        }
        // List tickets
        case 12: {
            json tickets = listTickets();
            int counter = 1;

            cout << "Tickets:" << endl;

            for (auto& item : tickets) {
                cout << counter << ". " << "Ticket Issue: " << item["issue"].get<string>()  << ", Status: " << item["ticketStatus"].get<string>() << ", Opened by: " << item["reportedBy"].get<string>() << endl;
                counter++;
            }

            system("pause");

            break;
        }
        case 13: {
            return -1;
        }
        // Exit with status code 0
        case 14: {
            return 0;
        }
        default: {
            cout << "Invalid option." << endl;
            break;
        }
    }

    return 1;
}

int main() {
    bool isRunning = true;
    bool isLoggedUser = false;
    bool isClient = false;
    bool isEmployee = false;
    bool isAdmin = false;
    json user = json::parse(R"(
      {
        "name": "",
        "address": "",
        "email": "",
        "password": "",
        "phone": "",
        "type": ""
      }
    )");

    setupSoftware();

    while (isRunning) {
        if (!isLoggedUser) {
            int status = showGuestMenu(user);

            if (status == 0) {
                isRunning = false;
            }

            if (status == 1) {
                isLoggedUser = true;
                isClient = true;
            } else if (status == 2) {
                isLoggedUser = true;
                isEmployee = true;
            } else if (status == 3) {
                isLoggedUser = true;
                isAdmin = true;
            }
        } else {
            if (isClient) {
                int status = showCostumerMenu(user);

                if (status == 0) {
                    isRunning = false;
                }

                if (status == -1) {
                    isLoggedUser = false;
                    isClient = false;
                    user = json::parse(R"(
                      {
                        "name": "",
                        "address": "",
                        "email": "",
                        "password": "",
                        "phone": "",
                        "type": ""
                      }
                    )");
                }
            } else if (isEmployee) {
                cout << "|--------------WELCOME EMPLOYEE--------------|" << endl;
                int status = showEmployeeMenu(user);

                if (status == 0) {
                    isRunning = false;
                } else if (status == -1) {
                    isLoggedUser = false;
                    isEmployee = false;
                    user = json::parse(R"(
                      {
                        "name": "",
                        "address": "",
                        "email": "",
                        "password": "",
                        "phone": "",
                        "type": ""
                      }
                    )");
                }
            } else if (isAdmin) {
                cout << "|--------------WELCOME ADMIN--------------|" << endl;

                int status = showAdminMenu(user);

                if (status == 0) {
                    isRunning = false;
                } else if (status == -1) {
                    isLoggedUser = false;
                    isAdmin = false;
                    user = json::parse(R"(
                      {
                        "name": "",
                        "address": "",
                        "email": "",
                        "password": "",
                        "phone": "",
                        "type": ""
                      }
                    )");
                }
            }
        }
    }

    return 0;
}

//
// Created by yonatan on 11/8/18.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/Restaurant.h"


Restaurant::Restaurant() : nextCustomerId(0), open(true) {}


//todo:refactor split code
Restaurant::Restaurant(const std::string &configFilePath) :
        nextCustomerId(0), open(true) {
    std::vector<std::string> lines = getLines(configFilePath);
    std::vector<std::string> filteredLines;

    for (auto line : lines) {
        if (!line.empty() && line.find('#') == std::string::npos) {
            filteredLines.push_back(line);
        }
    }


    std::string capacitiesString = filteredLines[1];
    std::vector<std::string> capacities = split(capacitiesString, ',');

    for (auto size_s: capacities) {
        int capacity = std::stoi(size_s);
        Table *table = new Table(capacity);
        tables.push_back(table);
    }

    // making the menu
    int tempNextDishId = 0;
    for (unsigned i = 2; i < filteredLines.size(); ++i) {
        std::string line = filteredLines[i];
        std::vector<std::string> params = split(line, ',');
        std::string d_name = params[0];
        int d_price = std::stoi(params[2]);
        int d_id = tempNextDishId;
        std::string type_s = params[1];
        DishType d_type = getType(type_s);
        menu.push_back(Dish(d_id, d_name, d_price, d_type));
        ++tempNextDishId;
    }
    //important
    //
    open = true;
}

DishType Restaurant::getType(std::string &type_s) {
    DishType d_type;
    if (type_s == "VEG") {
        d_type = VEG;
    } else if (type_s == "SPC") {
        d_type = SPC;
    } else if (type_s == "BVG") {
        d_type = BVG;
    } else if (type_s == "ALC") {
        d_type = ALC;
    }
    return d_type;
}

std::vector<std::string> Restaurant::getLines(const std::string &configFilePath) {
    std::ifstream t(configFilePath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string content = buffer.str();
    return split(content, '\n');
}

void Restaurant::start() {
    std::cout << "Restaurant is now open!" << std::endl;
    while (open) {
        // assuming input is the correct format for each command!!
        // as described in assignment page
        BaseAction *action = nullptr;
        bool handleCustIds = false;
        std::string cmd;
        std::getline(std::cin, cmd);
        std::vector<std::string> words = split(cmd, ' ');
        int tableId;
        std::vector<Customer *> customers;
        if (words[0] == "closeall") { //close all
            action = new CloseAll();
        } else if (words[0] == "open") { //open table
            handleCustIds = true; //upon error in open table reset customer id count
            tableId = std::stoi(words[1]);
            initiateCustomersByType(words, customers);
            action = new OpenTable(tableId, customers);
            cleanTempCustomers(customers);
        } else if (words[0] == "order") { //order from table-id
            tableId = std::stoi(words[1]);
            action = new Order(tableId);
        } else if (words[0] == "move") { // move customer
            int source = std::stoi(words[1]);
            int destination = std::stoi(words[2]);
            int customerId = std::stoi(words[3]);
            action = new MoveCustomer(source, destination, customerId);
        } else if (words[0] == "close") {
            tableId = std::stoi(words[1]);
            action = new Close(tableId);
        } else if (words[0] == "menu") { //print menu
            action = new PrintMenu();
        } else if (words[0] == "status") {
            tableId = std::stoi(words[1]);
            action = new PrintTableStatus(tableId);
        } else if (words[0] == "log") {
            action = new PrintActionsLog();
        } else if (words[0] == "backup") {
            action = new BackupRestaurant();
        } else if (words[0] == "restore") {
            action = new RestoreResturant();
        }
        if (action != nullptr) {
            // BaseAction destructor and its derivatives should clean memory mass here
            action->act(*this);
            actionsLog.push_back(action);
            if (action->getStatus() == ERROR && handleCustIds) {
                handleCustomerIdsGeneration((int) customers.size());
            }
        }
    }
}

void Restaurant::cleanTempCustomers(std::vector<Customer *> &customers) {
    for (auto cust: customers) {
        delete cust;
    }
    customers.clear();
}

void Restaurant::initiateCustomersByType(const std::vector<std::string> &words, std::vector<Customer *> &customers) {
    if (words.size() >= 2) {
        for (int i = 2; i < (int) words.size(); ++i) {
            // customer section
            std::string customer = words[i];
            std::vector<std::string> pair = split(customer, ',');
            std::string c_name = pair[0];
            std::string c_type = pair[1];
            int c_id = generateCustomerId();
            if (c_type == "veg") {
                customers.push_back(new VegetarianCustomer(c_name, c_id));
            } else if (c_type == "chp") {
                customers.push_back(new CheapCustomer(c_name, c_id));
            } else if (c_type == "spc") {
                customers.push_back(new SpicyCustomer(c_name, c_id));
            } else {
                customers.push_back(new AlchoholicCustomer(c_name, c_id));
            }
        }
    }
}

int Restaurant::getNumOfTables() const {
    return (int) tables.size();
}

Table *Restaurant::getTable(int ind) {
    if (ind <= (int) tables.size()) {
        return tables[ind];
    } else {
        return nullptr;
    }
}

const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
    return actionsLog;
}

std::vector<Dish> &Restaurant::getMenu() {
    return menu;
}

std::vector<std::string> Restaurant::split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int Restaurant::generateCustomerId() {
    int idToReturn = nextCustomerId;
    ++nextCustomerId;
    return idToReturn;
}

Restaurant::~Restaurant() {
    for (auto table: tables) {
        delete table;
    }
    tables.clear();
    for (auto action: actionsLog) {
        delete action;
    }
    actionsLog.clear();
}

void Restaurant::closeRestuarant() {
    open = false;
}


Restaurant::Restaurant(const Restaurant &other) {
    // im empty.
    // copy everything from other and put it in me
    // other keeps living
    copyFromOtherIntoMe(other);
}

Restaurant &Restaurant::operator=(const Restaurant &other) {
    // im existing.
    // replace everything in myself with copies from other
    // other keeps living
    if (&other != this) {
        cleanMySelf();
        copyFromOtherIntoMe(other);
    }
    return *this;
}

Restaurant::Restaurant(Restaurant &&other) {
    // im empty.
    // other will disappear and im stealing everything
    StealFromOtherToMe(other);
    cleanOther(other);
}

Restaurant &Restaurant::operator=(Restaurant &&other) {
    // im existing.
    // other will disappear and im stealing everything
    if (&other != this) {
        cleanMySelf();
        StealFromOtherToMe(other);
        cleanOther(other);
    }

    return *this;
}


void Restaurant::cleanMySelf() {
    for (unsigned i = 0; i < tables.size(); ++i) {
        delete tables[i];
    }
    tables.clear();
    for (unsigned i = 0; i < actionsLog.size(); ++i) {
        delete actionsLog[i];
    }
    actionsLog.clear();
    menu.clear();
}


void Restaurant::copyFromOtherIntoMe(const Restaurant &other) {
    nextCustomerId = other.nextCustomerId;
    open = other.open;
    // we are generating new tables!
    for (auto table: other.tables) {
        tables.push_back(new Table(*table));
    }
    // we are generating new actions!
    for (auto action: other.actionsLog) {
        actionsLog.push_back(action->clone());
    }
    for (const auto &dish: other.menu) {
        menu.push_back(dish);
    }
}

//todo:ask joni why isnt it the same logic as here above
void Restaurant::StealFromOtherToMe(const Restaurant &other) {
    nextCustomerId = other.nextCustomerId;
    open = other.open;
    for (auto table : other.tables) {
        tables.push_back(table);
    }
    for (auto action: other.actionsLog) {
        actionsLog.push_back(action);
    }
    for (const auto &dish: other.menu) {
        menu.push_back(dish);
    }
}

void Restaurant::cleanOther(Restaurant &other) {
    for (unsigned i = 0; i < other.tables.size(); ++i) {
        other.tables[i] = nullptr;
        // no deleting here
    }
}

void Restaurant::handleCustomerIdsGeneration(int sizeToDecrease) {
    for (int i = 0; i < sizeToDecrease; ++i) {
        nextCustomerId--;
    }
}

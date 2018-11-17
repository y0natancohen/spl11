//
// Created by yonatan on 11/8/18.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/Restaurant.h"

extern Restaurant *backup;

Restaurant::Restaurant() : nextCustomerId(0), nextDishId(0), open(true) {}


//todo:refactor split code
Restaurant::Restaurant(const std::string &configFilePath) {
    std::string errMsg = "config file not good";
    std::vector<std::string> lines = getLines(configFilePath);
    // going over the lines
    int num_of_tables_index = 0;
    int tables_desc_index = 0;
    int menu_index = 0;
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i] == "#number of tables") {
            num_of_tables_index = i + 1;
        }
        if (lines[i] == "#tables description") {
            tables_desc_index = i + 1;
        }
        if (lines[i] == "#Menu") {
            menu_index = i + 1;
        }
    }
    // making the tables
    int num_tables = std::stoi(lines[num_of_tables_index]);
    std::string table_desc_s = lines[tables_desc_index];
    std::vector<std::string> sizes = split(table_desc_s, ',');
    if (sizes.size() != num_tables) {
        return;
    }
    for (auto size_s: sizes) {
        int size = std::stoi(size_s);
        Table *table = new Table(size);
        tables.push_back(table);
    }
    // making the menu
    int tempNextDishId = 0;
    for (int i = 0; i < lines.size(); ++i) {
        if (i >= menu_index) {  // menu lines
            std::string line = lines[i];
            std::vector<std::string> params = split(line, ',');
            std::string d_name = params[0];
            int d_price = std::stoi(params[2]);
            int d_id = tempNextDishId;
            std::string type_s = params[1];
            DishType d_type = getType(type_s);
            menu.push_back(Dish(d_id, d_name, d_price, d_type));
            ++tempNextDishId;
        }
    }
    //important
    nextDishId = tempNextDishId;
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
    } else {
        // TODO: error here
        std::cout << "received a bad type: " + type_s << std::endl;
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
        std::string cmd;
        std::getline(std::cin, cmd);
        std::vector<std::string> words = split(cmd, ' ');
        int tableId = -1;
        if (words[0] == "closeall") { //close all
            action = new CloseAll();
        } else if (words[0] == "open") { //open table
            tableId = std::stoi(words[1]);
            std::vector<Customer *> customers;
            initiateCustomersByType(words, customers);
            action = new OpenTable(tableId, customers);
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
            actionsLog.push_back(action->clone());
        }
    }
}

void Restaurant::initiateCustomersByType(const std::vector<std::string> &words, std::vector<Customer *> &customers) {
    if (words.size() >= 2) {
        std::vector<std::string> customersInPairs = split(words[2], ' ');
        for (int i = 0; i < customersInPairs.size(); ++i) {
            // customer section
            std::string customer = customersInPairs[i];
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
            } else if (c_type == "alc") {
                customers.push_back(new AlchoholicCustomer(c_name, c_id));
            }
        }
    }
}

int Restaurant::getNumOfTables() const {
    return tables.size();
}

Table *Restaurant::getTable(int ind) {
    if (ind <= tables.size()) {
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
    std::cout << "Restaurant is now closed!" << std::endl;
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
    for (int i = 0; i < tables.size(); ++i) {
        delete tables[i];
    }
    tables.clear();
    for (int i = 0; i < actionsLog.size(); ++i) {
        delete actionsLog[i];
    }
    tables.clear();
    actionsLog.clear();
}


void Restaurant::copyFromOtherIntoMe(const Restaurant &other) {
    nextDishId = other.nextDishId;
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
}

void Restaurant::StealFromOtherToMe(const Restaurant &other) {
    nextDishId = other.nextDishId;
    nextCustomerId = other.nextCustomerId;
    open = other.open;
    for (auto table : other.tables) {
        tables.push_back(table);
    }

    for (auto action: other.actionsLog) {
        actionsLog.push_back(action);
    }
}

void Restaurant::cleanOther(Restaurant &other) {
    for (int i = 0; i < other.tables.size(); ++i) {
        other.tables[i] = nullptr;
        // no deleting here
    }
}
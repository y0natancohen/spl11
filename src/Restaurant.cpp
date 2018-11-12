//
// Created by yonatan on 11/8/18.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/Restaurant.h"

Restaurant::Restaurant() : nextCustomerId(0), nextDishId(0) {}

//todo:refactor split code
Restaurant::Restaurant(const std::string &configFilePath) {
//    nextCustomerId = 0;
//    nextDishId = 0;
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
    print("num of tables is", num_tables);
    std::string table_desc_s = lines[tables_desc_index];
    std::vector<std::string> sizes = split(table_desc_s, ',');
    if (sizes.size() != num_tables) {
        std::cout << errMsg << std::endl;
        print("got ", sizes.size());
        print("number of tables is ", num_tables);
        // TODO: error here?
        return;
    }
    for (auto size_s: sizes) {
        int size = std::stoi(size_s);
        print("making table with seats ", size);
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

    if (false) {
        // TODO: more verifying here?
        // TODO: error here?
        std::cout << errMsg << std::endl;
        return;
    } else {

    }
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

std::vector<std::string> Restaurant::getLines(
        const std::string &configFilePath) {
    std::ifstream t(configFilePath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string content = buffer.str();
    return split(content, '\n');
}

void Restaurant::start() {
    open = true;
    std::cout << "Restaurant is now open!" << std::endl;
    while (running) {
        std::string cmd;
        std::getline(std::cin, cmd);
        std::cout << cmd << std::endl;
        BaseAction *action;
        std::vector<std::string> words = split(cmd, ' ');
        int tableId = std::stoi(words[1]);
        if (cmd == "closeall") { //close all
            action = new CloseAll(cmd);
        }
        if (startsWith(cmd, "open")) { //open table
            print("table id is", tableId);
            std::vector<Customer *> customers;
            initiateCustomersByType(words, customers);
            action = new OpenTable(tableId, customers);
        } else if (startsWith(cmd, "order")) { //order from table-id
            std::cout << "received order" << std::endl;
            Order order = Order(tableId, cmd);
        } else if (startsWith(cmd, "move")) { // move customer
            std::cout << "received move" << std::endl;
            //action = new MoveCustomer(/*todo:split command to params*/)
        } else if (startsWith(cmd, "close ")) { // no collision with closeall because it was before
            std::cout << "received close " << std::endl;
            action = new Close(tableId, cmd);
        } else if (cmd == "menu") { //print menut
            action = new PrintMenu(cmd);
        } else if (cmd == "status") { //print table status
            action = new PrintTableStatus(tableId, cmd);
        } else if (cmd == "log") { //print actions log
            action = new PrintActionsLog(cmd);
        } else if (cmd == "backup") { //backup restaurant
            action = new BackupRestaurant(cmd);
        } else if (cmd == "restore") { //restore restaurant
            action = new RestoreResturant(cmd);
        } else {
            // TODO: error
            open = false;
            std::cout << "no such" << std::endl;
        }
        // BaseAction destructor and its derivatives should clean memory mass here
        if (!open) { action->act(*this); };
    }
}

void Restaurant::initiateCustomersByType(const std::vector<std::string> &words, std::vector<Customer *> &customers) {
    for (int i = 0; i < words.size(); ++i) {
        if (i >= 2) { // customer section
            std::string customer = words[i];
            std::vector<std::string> pair = split(customer, ',');
            std::string c_name = pair[0];
            std::string c_type = pair[1];
            int c_id = generateCustomerId();
            if (c_type == "veg") {
                customers.push_back(new VegetarianCustomer(c_name, c_id));
                std::cout << "making a veg" << std::endl;
            } else if (c_type == "chp") {
                customers.push_back(new CheapCustomer(c_name, c_id));
                std::cout << "making a cheap" << std::endl;
            } else if (c_type == "spc") {
                customers.push_back(new SpicyCustomer(c_name, c_id));
                std::cout << "making a spicy" << std::endl;
            } else if (c_type == "alc") {
                customers.push_back(new AlchoholicCustomer(c_name, c_id));
                std::cout << "making a alcoholic" << std::endl;
            } else {
                print("unknown customer type - " + c_type); // TODO: what here?
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


int Restaurant::generateDishId() {
    int idToReturn = nextDishId;
    ++nextDishId;
    return idToReturn;
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

bool Restaurant::startsWith(std::string a_string, std::string prefix) {
    int len = prefix.size();
    if (a_string.size() < len) {
        return false;
    } else {
        auto res = std::mismatch(prefix.begin(), prefix.end(), a_string.begin());

        return (res.first == prefix.end());
    }

}

void Restaurant::print(std::string s, int i) {
    std::cout << s + std::to_string(i) << std::endl;
}

void Restaurant::print(std::string s) {
    std::cout << s << std::endl;

}


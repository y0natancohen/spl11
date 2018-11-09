//
// Created by yonatan on 11/8/18.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/Restaurant.h"

Restaurant::Restaurant() {
//    nextCustomerId = 0;
//    nextDishId = 0;
}


Restaurant::Restaurant(const std::string &configFilePath) {
//    nextCustomerId = 0;
//    nextDishId = 0;
    std::string errMsg= "config file not good";
    std::vector<std::string> lines = getLines(configFilePath);

    // going over the lines

    int num_tables_index = 0;
    int tables_desc = 0;
    int tables_desc_index = 0;
    int menu_index = 0;

    for (int i = 0; i < lines.size(); ++i) {
//        std::cout << i << std::endl;
//        std::cout << lines[i] << std::endl;
        if (lines[i] == "#number of tables"){
            num_tables_index = i + 1;
        }
        if (lines[i] == "#tables description"){
            tables_desc_index = i + 1;
        }
        if (lines[i] == "#Menu"){
            menu_index = i + 1;
//            std::cout << "menu starts at line " << std::endl;
//            std::cout << menu_index << std::endl;
        }
    }

    // making the tables

    int num_tables = std::stoi(lines[num_tables_index]);

    std::string table_desc_s = lines[tables_desc_index];
    std::vector<std::string> sizes = split(table_desc_s, ',');

    if (sizes.size() != num_tables){
        std::cout << errMsg << std::endl;
        std::cout << "got" << std::endl;
        std::cout << sizes.size() << std::endl;
        std::cout << "number of tables is" + num_tables << std::endl;
        // TODO: error here?
        return;
    }
    for (auto size_s: sizes){
        int size = std::stoi(size_s);
        Table *table = new Table(size);
        tables.push_back(table);
    }

    // making the menu
    int tempNextDishId = 0;

    for (int i = 0; i < lines.size(); ++i) {
        if (i >= menu_index){  // menu lines
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

    if (false){
        // TODO: more verifying here?
        // TODO: error here?
        std::cout << errMsg << std::endl;
        return;
    } else{

    }


}

DishType Restaurant::getType(std::string &type_s) {
    DishType d_type;
    if (type_s == "VEG"){
                d_type = VEG;
            }else if (type_s == "SPC"){
                d_type = SPC;
            }else if (type_s == "BVG"){
                d_type = BVG;
            }else if (type_s == "ALC"){
                d_type = ALC;
            }else{
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
//    std::cout << "config content" << std::endl;
//    std::cout << content << std::endl;
//    std::cout << "config content" << std::endl;
    return split(content, '\n');
}

void Restaurant::start() {
    open = true;
    std::cout << "Restaurant is now open!" << std::endl;
    bool keepGoing = true;
    while (running){
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "closeall"){
            // TODO: more things
            stop();
        }

        else if(startsWith(cmd, "open")){
            std::vector<std::string> words = split(cmd, ' ');
            int tableId = std::stoi(words[1]);
            std::vector<Customer *> customers;

            for (int i = 0; i < words.size(); ++i) {
                if (i >= 2){ // customer section
                    std::string customer = words[i];
                    std::vector<std::string> pair = split(customer, ',');
                    std::string c_name = pair[0];
                    std::string c_type = pair[1];
                    int c_id = generateCustomerId();
                    if (c_type == "veg"){
                        customers.push_back(new VegetarianCustomer(c_name, c_id));
                    }else if (c_type == "chp"){
                        customers.push_back(new CheapCustomer(c_name, c_id));
                    } else if (c_type == "spc"){
                        customers.push_back(new SpicyCustomer(c_name, c_id));
                    } else if (c_type == "alc"){
                        customers.push_back(new AlchoholicCustomer(c_name, c_id));
                    }
                }
            }
            OpenTable ot = OpenTable(tableId, customers);
            ot.act(*this);
        }

        else if(startsWith(cmd, "order")){
            std::string errMsg = "Table does not exist or is not open";
            std::vector<std::string > words = split(cmd, ' ');
            int tableNum = std::stoi(words[1]);
            Order order = Order(tableNum);
            order.act(*this);

            // order act?
            // order complete?

        }

        else if(cmd == "open table table table"){

        }

        else if(cmd == "open table table table"){

        }

        else if(cmd == "open table table table"){

        }

        else if(cmd == "open table table table"){

        } else{
            // TODO: error
            std::cout << "no such" << std::endl;
        }
    }
}

int Restaurant::getNumOfTables() const {
    return tables.size();
}

Table *Restaurant::getTable(int ind) {
    return tables[0];
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
    while (std::getline(tokenStream, token, delimiter))
    {
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
    for (auto table: tables){
        delete table;
    }
    tables.clear();
    for (auto action: actionsLog){
        delete action;
    }
    actionsLog.clear();
    std::cout << "Restaurant is now closed!" << std::endl;


}

void Restaurant::stop() {
    running = false;
}

bool Restaurant::startsWith(std::string a_string, std::string prefix) {
    int len = prefix.size();
    if (a_string.size() < len){
        return false;
    }else{
        auto res = std::mismatch(prefix.begin(), prefix.end(), a_string.begin());

        return (res.first == prefix.end());
    }

}



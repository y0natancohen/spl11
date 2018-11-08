//
// Created by yonatan on 11/8/18.
//

#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id):
                name(c_name), id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

// VEG

VegetarianCustomer::VegetarianCustomer(std::string name, int id):
                    Customer(name, id) {}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    return std::vector<int>();
}

std::string VegetarianCustomer::toString() const {
    return getName();
}

// CHEAP


CheapCustomer::CheapCustomer(std::string name, int id) :
                Customer(name, id) {}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    return std::vector<int>();
}

std::string CheapCustomer::toString() const {
    return getName();
}

// SPICY


SpicyCustomer::SpicyCustomer(std::string name, int id) :
                Customer(name, id) {}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    return std::vector<int>();
}

std::string SpicyCustomer::toString() const {
    return getName();
}

// ALCOHOL

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) :
                Customer(name, id) {}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    return std::vector<int>();
}

std::string AlchoholicCustomer::toString() const {
    return getName();
}

//
// Created by yonatan on 11/8/18.
//

#include "../include/Table.h"
#include <iostream>


Table::Table(int t_capacity): capacity(t_capacity) {
    open = false;
}

int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer *customer) {
    // TODO: more things here?
    customersList.push_back(customer);

}

void Table::removeCustomer(int id) {
    for (int i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id){
            delete customersList[i];
            // TODO: implement delete customer
            customersList.erase(customersList.begin() + i);
        }
    }
}

Customer* Table::getCustomer(int id) {
    for (int i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id){
            return customersList[i];
        }
    }
    std::cout << "somthing bad happend 123" << std::endl;
    return nullptr;
}

std::vector<Customer *> &Table::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Table::getOrders() {
    return orderList;
}

void Table::order(const std::vector<Dish> &menu) {
//    std::cout << "inside Table.order"<< std::endl;
//    std::cout << customersList.size() << std::endl;
    for (auto customer: customersList) {
//        std::cout << "working on customer"<< std::endl;
//        std::cout <<  customer->getName()<< std::endl;
//        std::cout << customer->getName() + " ordering"<< std::endl;
//        bool cond = true;
//        if (cond){
//            std::cout << customer->getDrinksHad() << std::endl;
//        }

        std::vector<int> dish_ids = customer->order(menu);

//        std::cout << customer->getName() + " ordered"<< std::endl;
        for (auto dish_id: dish_ids){
            orderList.push_back(
                    OrderPair(customer->getId(), getDish(menu, dish_id))
                    );
            std::string print_s = customer->getName() + " ordered " + getDish(menu, dish_id).getName();
            std::cout <<  print_s << std::endl;
        }
    }

}

Dish Table::getDish(const std::vector<Dish> &menu, int d_id) {
    for (auto dish:menu){
        if (dish.getId() == d_id){
            return dish;
        }
    }
}

void Table::openTable() {
    // TODO: this

}

void Table::closeTable() {
    // TODO: this
}

int Table::getBill() {
    int sum = 0;
    for (int i = 0; i < orderList.size(); ++i) {
        sum = sum + orderList[i].second.getPrice();
    }
    return sum;
}

bool Table::isOpen() {
    return open;
}

Table::~Table() {
    for (auto customer: customersList){
        delete customer;
    }
    customersList.clear();
}

void Table::doOpen() {
    open = true;
}

void Table::doClose() {
    open = false;
}

void Table::clearCustomers() {
    customersList.clear();
}

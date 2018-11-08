//
// Created by yonatan on 11/8/18.
//

#include "../include/Table.h"
#include <iostream>


Table::Table(int t_capacity): capacity(t_capacity) {}

int Table::getCapacity() const {
    return capacity;
}

void Table::addCustomer(Customer *customer) {
    // TODO: more things here
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
    // TODO: this

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


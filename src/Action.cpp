//
// Created by yonatan on 11/8/18.
//

#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"
#include "../include/Customer.h"
#include "../include/Dish.h"

//BaseAction::BaseAction() {
//    status = PENDING;
//}

BaseAction::BaseAction(std::string _cmd) {
    status = PENDING;
    cmd = _cmd;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::error(std::string errorMsg) {
    std::string msg = "Error: " + errorMsg;
    std::cout << msg << std::endl;
    status = ERROR;
    this->errorMsg = msg;
}

void BaseAction::complete() {
    status = COMPLETED;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

std::string BaseAction::baseToString() const {
    if (status == COMPLETED){
        return cmd;
    } else if (status == ERROR){
        return cmd + "\n" + errorMsg;
    }
}



//OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):
//                    BaseAction(),
//                    tableId(id), customers(customersList) {
//    std::cout << "elad and yonatan not supposed to be here" << std::endl;
//}

OpenTable::OpenTable(int id, std::vector<Customer *> &customersList, std::string cmd):
                    BaseAction(cmd), tableId(id),
                    customers(customersList) {  // at this stage customerList is empty,
                                                // but it will be filled, it is pointing to Table.customers
//    for (auto customer : customersList){
//        customers.push_back(customer);
//    }
    //TODO: maybe copying the customers one by one?
}


void OpenTable::act(Restaurant &restaurant) {
    std::cout << "acting open table" << std::endl;
    Table* table = restaurant.getTable(tableId);

    if (table == nullptr){
        restaurant.print("", 1);
        error("Table does not exist or is already open");
        return;
    }
    if (table->isOpen()){
        restaurant.print("", 2);
        error("Table does not exist or is already open");
        return;
    }

    complete();

}

std::string OpenTable::toString() const {
    std::string s = "a very nice open table";
    return s;
}

OpenTable::~OpenTable() {
    for (auto customer : customers){
        delete customer;
    }
//    customers.clear();
// customers is const
}

//void OpenTable::clearCustomers() {
//    customers.clear();
//}


//Order::Order(int id): tableId(id) {}


Order::Order(int id, std::string cmd):
        BaseAction(cmd),
        tableId(id) {}

void Order::act(Restaurant &restaurant) {
    std::cout << "acting order" << std::endl;
    Table* table = restaurant.getTable(tableId);

    if (table == nullptr){
        std::cout << 1 << std::endl;
        error("Table does not exist or is not open");
        return;
    }
    if (not table->isOpen()){
        std::cout << 2 << std::endl;
        error("Table does not exist or is not open");
        return;
    }

    std::cout << 3 << std::endl;
    table->order(restaurant.getMenu());
    std::cout << 4 << std::endl;
    complete();
}

std::string Order::toString() const {
    std::string s = "a very nice order";
    return s;
}

// close

Close::Close(int id, std::string cmd):
        BaseAction(cmd),
        tableId(id) {}

void Close::act(Restaurant &restaurant) {

}

std::string Close::toString() const {
    std::string s = "a very nice close table";
    return s;
}

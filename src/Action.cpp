//
// Created by yonatan on 11/8/18.
//

#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"
#include "../include/Customer.h"
#include "../include/Dish.h"

BaseAction::BaseAction() {
    status = PENDING;
}

BaseAction::BaseAction(std::string _cmd) {

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
    if (status == COMPLETED) {
        return cmd;
    } else if (status == ERROR) {
        return cmd + "\n" + errorMsg;
    }
}

BaseAction::~BaseAction() {

}

Order::Order(int id, std::string cmd) :
        tableId(id) {}

void Order::act(Restaurant &restaurant) {
    std::cout << "acting order" << std::endl;
    Table *table = restaurant.getTable(tableId);

    if (table == nullptr) {
        std::cout << 1 << std::endl;
        error("Table does not exist or is not open");
        return;
    }
    if (not table->isOpen()) {
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

Order::Order(int id) : tableId(id) {

}

// close table
Close::Close(int id, std::string cmd) :
        tableId(id) {}

void Close::act(Restaurant &restaurant) {

}

std::string Close::toString() const {
    std::string s = "a very nice close table";
    return s;
}

//open table
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : BaseAction(cmd), tableId(id),
                                                                       customers(customersList) {

}

std::string OpenTable::toString() const {
    std::string s = "a very nice open table";
    return s;
}


/**
 * opens a table by id and add customers to it
 * @param restaurant
 */
void OpenTable::act(Restaurant &restaurant) {
    std::cout << "acting open table" << std::endl;
    Table *table = restaurant.getTable(tableId);
    if (table == nullptr) {
        restaurant.print("", 1);
        error("Table does not exist or is already open");
        return;
    }
    if (table->isOpen()) {
        restaurant.print("", 2);
        error("Table does not exist or is already open");
        return;
    }
    for (auto customer : customers) {
        table->addCustomer(customer);
    }
    table->openTable();
    complete();

}

//restore
RestoreResturant::RestoreResturant() {

}

RestoreResturant::RestoreResturant(std::string cmd) : BaseAction(cmd) {

}

void RestoreResturant::act(Restaurant &restaurant) {

}

std::string RestoreResturant::toString() const {
    return std::string();
}

//backup
std::string BackupRestaurant::toString() const {
    return std::string();
}

void BackupRestaurant::act(Restaurant &restaurant) {

}

BackupRestaurant::BackupRestaurant(std::string cmd) : BaseAction(cmd) {

}

BackupRestaurant::BackupRestaurant() {

}

//actions log
std::string PrintActionsLog::toString() const {
    return std::string();
}

void PrintActionsLog::act(Restaurant &restaurant) {

}

PrintActionsLog::PrintActionsLog(std::string cmd) : BaseAction(cmd) {

}

//status
std::string PrintTableStatus::toString() const {
    return std::string();
}

void PrintTableStatus::act(Restaurant &restaurant) {

}

PrintTableStatus::PrintTableStatus(int id, std::string cmd) : tableId(id) {

}

PrintTableStatus::PrintTableStatus(int id) : tableId(id) {

}

//print menu
std::string PrintMenu::toString() const {
    return std::string();
}

void PrintMenu::act(Restaurant &restaurant) {

}

PrintMenu::PrintMenu(std::string cmd) : BaseAction(cmd) {

}

PrintMenu::PrintMenu() {

}

std::string CloseAll::toString() const {
    return std::string();
}

void CloseAll::act(Restaurant &restaurant) {

    restaurant.closeRestuarant();
}

CloseAll::CloseAll(std::string cmd) : BaseAction(cmd) {

}

CloseAll::CloseAll() {

}

std::string MoveCustomer::toString() const {
    return std::string();
}

void MoveCustomer::act(Restaurant &restaurant) {

}

MoveCustomer::MoveCustomer(int src, int dst, int customerId, std::string cmd) : srcTable(src), dstTable(dst),
                                                                                id(customerId) {

}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src), dstTable(dst), id(customerId) {

}

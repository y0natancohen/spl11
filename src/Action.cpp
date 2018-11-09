//
// Created by yonatan on 11/8/18.
//

#include "../include/Action.h"

BaseAction::BaseAction() {

}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::error(std::string errorMsg) {
    std::string msg = "Error: " + errorMsg;
    std::cout << msg << std::endl;
}

void BaseAction::complete() {

}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):
                    tableId(id), customers(customersList) {}

void OpenTable::act(Restaurant &restaurant) {

}

std::string OpenTable::toString() const {
    std::string s = "a very nice open table";
    return s;
}

Order::Order(int id): tableId(id) {}

void Order::act(Restaurant &restaurant) {
//    if (restaurant.tables.size() <= tableNum){
//        error(errMsg);
//    } else if !(tables[tableNum]->isOpen()){
//        order.error(errMsg);
//    } else{
//        for (int i = 0; i < tables[tableNum]->getCustomers().size(); ++i) {
//            tables[tableNum]->getCustomers()[i]->order(menu);
//        }
//    }
}

std::string Order::toString() const {
    std::string s = "a very nice order";
    return s;
}

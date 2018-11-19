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


ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::error(std::string errorMsg) {
    std::string msg = "Error: " + errorMsg;
    status = ERROR;
    std::cout << msg << std::endl;
    this->errorMsg = msg;
}

void BaseAction::complete() {
    status = COMPLETED;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::~BaseAction() {

}

std::string BaseAction::getStatusValue(ActionStatus actionStatus) const {
    if (actionStatus == COMPLETED) {
        return "Completed";
    } else if (actionStatus == PENDING) {
        return "Pending";
    } else {
        return "Error";
    }
}

BaseAction::BaseAction(const BaseAction &other) :
        errorMsg(other.errorMsg), status(other.status) {}

Order::Order(int id) : tableId(id) {

}


void Order::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(tableId);
    if (table == nullptr) {
        error("Table does not exist or is not open");
        return;
    }
    if (!table->isOpen()) {
        error("Table does not exist or is not open");
        return;
    }
    table->order(restaurant.getMenu());
    complete();
}

std::string BaseAction::endOfToString(ActionStatus actionStatus,
                                      std::string errMsg) const {
    if (actionStatus == ERROR) {
        return " " + errMsg;
    }
    return " " + getStatusValue(actionStatus);
}

std::string Order::toString() const {
    std::string s = "order " + std::to_string(tableId)
                    + endOfToString(getStatus(), getErrorMsg());
    return s;
}

BaseAction *Order::clone() {
    return new Order(*this);
}


// close table

Close::Close(int id) : tableId(id) {

}

void Close::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(tableId);
    if (table == nullptr) {
        error("Table does not exist or is not open");
        return;
    }
    if (not table->isOpen()) {
        error("Table does not exist or is not open");
        return;
    }
    int bill = table->getBill();
    std::string msg = "Table " + std::to_string(tableId) +
                      " was closed. Bill " + std::to_string(bill) + "NIS";
    std::cout << msg << std::endl;
    table->closeTable();
    complete();
}

std::string Close::toString() const {
    return "close " + std::to_string(tableId)
           + endOfToString(getStatus(), getErrorMsg());
}

BaseAction *Close::clone() {
    return new Close(*this);
}


//open table
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id) {
    for (auto customer: customersList) {
        customers.push_back(customer->clone());
    }
}

//todo: cut last char

std::string OpenTable::toString() const {
    std::string msg = "open " + std::to_string(tableId) + " ";
    bool deleteLastSpace = false;
    for (const auto &customer : customers) {
        msg += customer->getName() + ",";
        msg += customer->getType();
        msg += " ";
        deleteLastSpace = true;
    }
    if (deleteLastSpace) {
        msg = msg.substr(0, msg.size() - 1);
    }
    msg += endOfToString(getStatus(), getErrorMsg());
    return msg;
}

/**
 * opens a table by id and add customers to it
 * @param restaurant
 */
void OpenTable::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(tableId);
    if (table == nullptr) {
        error("Table does not exist or is already open");
        return;
    }
    if (table->isOpen()) {
        error("Table does not exist or is already open");
        return;
    }
    for (auto customer : customers) {
        // no need to verify capacity - forum
        // open table is managing its own memory from here
        table->addCustomer(customer->clone());
    }
    table->openTable();
    complete();
}

BaseAction *OpenTable::clone() {
    return new OpenTable(*this);
}

OpenTable::~OpenTable() {
    for (auto cust : customers) {
        delete cust;
    }
}

OpenTable::OpenTable(const OpenTable &other) :
        BaseAction(other), tableId(other.tableId) {

    for (auto customer: other.customers) {
        customers.push_back(customer->clone());
    }
}


//restore
RestoreResturant::RestoreResturant() {

}


void RestoreResturant::act(Restaurant &restaurant) {
    extern Restaurant *backup;
    if (backup == nullptr) {
        error("No backup available");
        return;
    } else {
        restaurant = *backup;
    }
    complete();
}

std::string RestoreResturant::toString() const {
    return "restore" + endOfToString(getStatus(), getErrorMsg());
}

BaseAction *RestoreResturant::clone() {
    return new RestoreResturant(*this);
}


//backup
std::string BackupRestaurant::toString() const {
    return "backup" + endOfToString(getStatus(), getErrorMsg());
}

void BackupRestaurant::act(Restaurant &restaurant) {
    extern Restaurant *backup;
    backup = new Restaurant(restaurant);
    complete();
}


BackupRestaurant::BackupRestaurant() {
}


BaseAction *BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}


//actions log
std::string PrintActionsLog::toString() const {
    return "log" + endOfToString(getStatus(), getErrorMsg());
}

void PrintActionsLog::act(Restaurant &restaurant) {
    for (const auto &actionLog : restaurant.getActionsLog()) {
        std::string log = actionLog->toString();
        std::cout << log << std::endl;
    }
    complete();
}


PrintActionsLog::PrintActionsLog() {}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

//status
std::string PrintTableStatus::toString() const {
    return "status " + std::to_string(tableId)
           + endOfToString(getStatus(), getErrorMsg());
}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(tableId);
    std::string currentStatus;
    if (!table->isOpen()) {
        currentStatus = " closed";
    } else {
        currentStatus = " open";
    }
    std::string status =
            "Table " + std::to_string(tableId) + " status:" + currentStatus;
    std::cout << status << std::endl;
    if (table->isOpen()) {
        std::cout << "Customers:" << std::endl;
        for (const auto &customer : table->getCustomers()) {
            //todo:elad move to toString of Customer
            std::string cust = std::to_string(customer->getId()) + " " + customer->getName();
            std::cout << cust << std::endl;
        }
        std::cout << "Orders:" << std::endl;
        for (const auto &orderPair : table->getOrders()) {
            std::string dish = orderPair.second.getName() + " " + std::to_string(orderPair.second.getPrice()) + "NIS " +
                               std::to_string(orderPair.first);
            std::cout << dish << std::endl;
        }
        std::cout << "Current Bill: " + std::to_string(table->getBill()) + "NIS" << std::endl;
    }
    complete();
}

PrintTableStatus::PrintTableStatus(int id) : tableId(id) {
}

BaseAction *PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}

//print menu
std::string PrintMenu::toString() const {
    return "menu" + endOfToString(getStatus(), getErrorMsg());
}

void PrintMenu::act(Restaurant &restaurant) {
    //todo: figure out how to fetch enum val instead of id
    for (const auto &dish : restaurant.getMenu()) {
        std::string msg =
                dish.getName() + " " + getTypeString(dish.getType()) + " " + std::to_string(dish.getPrice()) + "NIS";
        std::cout << msg << std::endl;
    }
    complete();
}


std::string PrintMenu::getTypeString(DishType t) {
    if (t == VEG) {
        return "VEG";
    } else if (t == SPC) {
        return "SPC";
    } else if (t == BVG) {
        return "BVG";
    } else {
        return "ALC";
    }
}


PrintMenu::PrintMenu() {

}

BaseAction *PrintMenu::clone() {
    return new PrintMenu(*this);
}


std::string CloseAll::toString() const {
    return "closeall" + endOfToString(getStatus(), getErrorMsg());
}

void CloseAll::act(Restaurant &restaurant) {
    for (int i = 0; i < restaurant.getNumOfTables() - 1; i++) {
        Table *t = restaurant.getTable(i);
        if (t->isOpen()) {
            Close *closeAct = new Close(i);
            closeAct->act(restaurant);
            delete closeAct;
        }
    }
    restaurant.closeRestuarant();
    complete();
}


CloseAll::CloseAll() {

}

BaseAction *CloseAll::clone() {
    return new CloseAll(*this);
}


std::string MoveCustomer::toString() const {
    return "move " + std::to_string(srcTable) + " " + std::to_string(dstTable) + " " +
           std::to_string(id) + endOfToString(getStatus(), getErrorMsg());
}

void MoveCustomer::act(Restaurant &restaurant) {
    Table *source = restaurant.getTable(srcTable);
    Table *dest = restaurant.getTable(dstTable);

    if ((source == nullptr) || (dest == nullptr)) {
        error("Cannot move customer ptrshit");
        return;
    }

    if ((!source->isOpen()) || (!dest->isOpen())) {
        error("Cannot move customer dest shit");
        return;
    }

    if ((int) dest->getCustomers().size() >= (int) dest->getCapacity()) { // table is full
        error("Cannot move customer capacity shit");
        return;
    }
    Customer *mover = source->getCustomer(id);
    if (mover == nullptr) {
        error("Cannot move customer no mover loser");
        return;
    }
    source->removeCustomer(id);
    dest->addCustomer(mover);
    std::vector<OrderPair> customerOrders = source->getCustomerOrders(id);
    source->removeCustomerOrders(id);
    dest->addOrders(customerOrders);
    if (source->getCustomers().empty()) { // table is empty
        source->closeTable();
    }
    complete();
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) :
        srcTable(src), dstTable(dst), id(customerId) {}

BaseAction *MoveCustomer::clone() {
    return new MoveCustomer(*this);
}



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

BaseAction::~BaseAction() {

}


Order::Order(int id) : tableId(id) {

}


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
    std::string s = "order " + std::to_string(tableId) + " " + std::to_string(getStatus());
    return s;
}

BaseAction *Order::clone() {
    return new Order(tableId);
}


// close table

Close::Close(int id) : tableId(id) {

}

void Close::act(Restaurant &restaurant) {
    std::cout << "acting close" << std::endl;
    Table *table = restaurant.getTable(tableId);

    if (table == nullptr) {
        std::cout << 5 << std::endl;
        error("Table does not exist or is not open");
        return;
    }
    if (not table->isOpen()) {
        std::cout << 6 << std::endl;
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
    return "close " + std::to_string(tableId) + " " + std::to_string(getStatus());
}

BaseAction *Close::clone() {
    return new Close(tableId);
}


//open table
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) :
        tableId(id), customers(customersList) {
    std::cout << "i was in Open table constructor" << std::endl;
}

std::string OpenTable::toString() const {
    std::string msg ="open " + std::to_string(tableId) + " ";
    for (const auto &customer : customers) {
        msg +=customer->getName() + ",";
        //todo:elad extract cust type
        //msg +=customer->;
    }
    return  msg + std::to_string(getStatus());
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
        // no need to verify capacity - forum
        table->addCustomer(customer);
    }
    table->openTable();
    complete();

}

BaseAction *OpenTable::clone() {
    return new OpenTable(tableId, customers);
}

OpenTable::~OpenTable() {
    for(auto cust : customers){
        delete cust;
    }
}

//restore
RestoreResturant::RestoreResturant() {

}


void RestoreResturant::act(Restaurant &restaurant) {

}

std::string RestoreResturant::toString() const {
    return "restore " + std::to_string(getStatus());
}

BaseAction *RestoreResturant::clone() {
    return new RestoreResturant();
}


//backup
std::string BackupRestaurant::toString() const {
    return "backup " + std::to_string(getStatus());
}

void BackupRestaurant::act(Restaurant &restaurant) {

}


BackupRestaurant::BackupRestaurant() {

}

BaseAction *BackupRestaurant::clone() {
    return new RestoreResturant();
}


//actions log
std::string PrintActionsLog::toString() const {
    return "";
}

void PrintActionsLog::act(Restaurant &restaurant) {
    for (const auto &actionLog : restaurant.getActionsLog()) {
        std::cout << actionLog->toString() << std::endl;
    }
}


PrintActionsLog::PrintActionsLog() {}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog();
}

//status
std::string PrintTableStatus::toString() const {
    return "status " + std::to_string(tableId) + " " + std::to_string(getStatus());
}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table *table = restaurant.getTable(tableId);
    std::string currentStatus = table->isOpen() == 1 ? " open" : " close";
    std::string status =
            "Table " + std::to_string(tableId) + " status:" + currentStatus;
    std::cout << status << std::endl;
    std::cout << "Customers:" << std::endl;
    for (const auto &customer : table->getCustomers()) {
        //todo:elad move to toString of Customer
        std::string cust = std::to_string(customer->getId()) + " " + customer->getName();
        std::cout << cust << std::endl;
    }
    std::cout << "Orders:" << std::endl;
    for (const auto &orderPair : table->getOrders()) {
        std::string dish = orderPair.second.getName() + " " + std::to_string(orderPair.second.getPrice()) + " " +
                           std::to_string(orderPair.first);
        std::cout << dish << std::endl;
    }
    std::cout << "Current Bill:" + std::to_string(table->getBill()) << std::endl;
}

PrintTableStatus::PrintTableStatus(int id) : tableId(id) {

}

BaseAction *PrintTableStatus::clone() {
    return new PrintTableStatus(tableId);
}


//print menu
std::string PrintMenu::toString() const {
    return "menu " + std::to_string(getStatus());
}

void PrintMenu::act(Restaurant &restaurant) {
    //todo: figure out how to fetch enum val instead of id
    for (const auto &dish : restaurant.getMenu()) {
        std::string msg =
                dish.getName() + " " + std::to_string(dish.getType()) + " " + std::to_string(dish.getPrice()) + "NIS";
        std::cout << msg << std::endl;
    }
}


PrintMenu::PrintMenu() {

}

BaseAction *PrintMenu::clone() {
    return new PrintMenu();
}


std::string CloseAll::toString() const {
    return "closeall " + std::to_string(getStatus());
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
}


CloseAll::CloseAll() {

}

BaseAction *CloseAll::clone() {
    return new CloseAll();
}


std::string MoveCustomer::toString() const {
    return "move " + std::to_string(srcTable) + " " + std::to_string(dstTable) + " " +
           std::to_string(id) + " " + std::to_string(getStatus());
}

void MoveCustomer::act(Restaurant &restaurant) {
    Table *source = restaurant.getTable(srcTable);
    Table *dest = restaurant.getTable(dstTable);

    if ((source == nullptr) || (dest == nullptr)) {
        error("Cannot move customer");
        std::cout << "because null" << std::endl;
        return;
    }

    if ((!source->isOpen()) || (!dest->isOpen())) {
        error("Cannot move customer");
        std::cout << "because not open" << std::endl;
        return;
    }

    if (dest->getCustomers().size() >= dest->getCapacity()) { // table is full
        error("Cannot move customer");
        std::cout << "because capacity " << std::endl;
        return;
    }

    Customer *mover = source->getCustomer(id);
    if (mover == nullptr) {
        error("Cannot move customer");
        std::cout << "because customer id is bad" << std::endl;
        return;
    }

    source->removeCustomer(id);
    dest->addCustomer(mover);

    std::vector<OrderPair> customerOrders = source->getCustomerOrders(id);
    source->removeCustomerOrders(id);
    dest->addOrders(customerOrders);

    if (source->getCustomers().empty()) { // table is empty
        Close closeActionInstance = Close(srcTable);
        closeActionInstance.act(restaurant);

    }
    complete();

}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) :
        srcTable(src), dstTable(dst), id(customerId) {}

BaseAction *MoveCustomer::clone() {
    return new MoveCustomer(srcTable,dstTable,id);
}



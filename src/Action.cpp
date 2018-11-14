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

std::string BaseAction::baseToString() const {
    if (status == COMPLETED) {
        return cmd;
    } else if (status == ERROR) {
        return cmd + "\n" + errorMsg;
    }
}

BaseAction::~BaseAction() {

}

BaseAction *BaseAction::generate(BaseAction &other) {
    return nullptr;
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
    std::string s = "a very nice order";
    return s;
}

BaseAction* Order::generate(Order &other) {
    return new Order(other.tableId);
}


// close table

Close::Close(int id): tableId(id) {

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
    std::string msg = "Table "  + std::to_string(tableId) +
            " was closed. Bill " + std::to_string(bill) + "NIS";
    std::cout << msg << std::endl;
    table->closeTable();
    complete();

}

std::string Close::toString() const {
    std::string s = "a very nice close table";
    return s;
}

BaseAction* Close::generate(Close &other) {
    return new Close(other.tableId);
}


//open table
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):
            tableId(id), customers(customersList) {
    std::cout << "i was in Open table constructor" << std::endl;
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
        // no need to verify capacity - forum
        table->addCustomer(customer);
    }
    table->openTable();
    complete();

}

BaseAction *OpenTable::generate(OpenTable &other) {
    return new OpenTable(other.tableId, customers);
}

//restore
RestoreResturant::RestoreResturant() {

}


void RestoreResturant::act(Restaurant &restaurant) {

}

std::string RestoreResturant::toString() const {
    return std::string();
}

BaseAction *RestoreResturant::generate(RestoreResturant &other) {
    return new RestoreResturant();
}

//backup
std::string BackupRestaurant::toString() const {
    return std::string();
}

void BackupRestaurant::act(Restaurant &restaurant) {

}


BackupRestaurant::BackupRestaurant() {

}

BaseAction *BackupRestaurant::generate(BackupRestaurant &other) {
    return new BackupRestaurant();
}

//actions log
std::string PrintActionsLog::toString() const {
    return std::string();
}

void PrintActionsLog::act(Restaurant &restaurant) {

}


BaseAction *PrintActionsLog::generate(PrintActionsLog &other) {
    return new PrintActionsLog();
}

PrintActionsLog::PrintActionsLog() {}

//status
std::string PrintTableStatus::toString() const {
    return std::string();
}

void PrintTableStatus::act(Restaurant &restaurant) {

}

PrintTableStatus::PrintTableStatus(int id) : tableId(id) {

}

BaseAction *PrintTableStatus::generate(PrintTableStatus &other) {
    return new PrintTableStatus(other.tableId);
}

//print menu
std::string PrintMenu::toString() const {
    return std::string();
}

void PrintMenu::act(Restaurant &restaurant) {

}


PrintMenu::PrintMenu() {

}

BaseAction *PrintMenu::generate(PrintMenu &other) {
    return new PrintMenu();
}

std::string CloseAll::toString() const {
    return std::string();
}

void CloseAll::act(Restaurant &restaurant) {

    restaurant.closeRestuarant();
}


CloseAll::CloseAll() {

}

BaseAction *CloseAll::generate(CloseAll &other) {
    return new CloseAll();
}

std::string MoveCustomer::toString() const {
    return std::string();
}

void MoveCustomer::act(Restaurant &restaurant) {
    Table *source = restaurant.getTable(srcTable);
    Table *dest = restaurant.getTable(dstTable);

    if ((source == nullptr) || (dest == nullptr)){
        error("Cannot move customer");
        std::cout << "because null"<< std::endl;
        return;
    }

    if ((! source->isOpen()) || (! dest->isOpen())){
        error("Cannot move customer");
        std::cout << "because not open"<< std::endl;
        return;
    }

    if (dest->getCustomers().size() >= dest->getCapacity()){ // table is full
        error("Cannot move customer");
        std::cout << "because capacity "<< std::endl;
        return;
    }

    Customer *mover = source->getCustomer(id);
    if (mover == nullptr){
        error("Cannot move customer");
        std::cout << "because customer id is bad"<< std::endl;
        return;
    }

    source->removeCustomer(id);
    dest->addCustomer(mover);

    std::vector<OrderPair> customerOrders = source->getCustomerOrders(id);
    source->removeCustomerOrders(id);
    dest->addOrders(customerOrders);

    if (source->getCustomers().empty()){ // table is empty
        Close closeActionInstance = Close(srcTable);
        closeActionInstance.act(restaurant);

    }
    complete();

}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):
            srcTable(src), dstTable(dst), id(customerId) {}

BaseAction *MoveCustomer::generate(MoveCustomer &other) {
    return new MoveCustomer(other.srcTable, other.dstTable, other.id);
}

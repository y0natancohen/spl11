#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus {
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction {
public:
    BaseAction();

//    BaseAction(std::string _cmd);

    virtual ~BaseAction();

    ActionStatus getStatus() const;

    virtual void act(Restaurant &restaurant) = 0;

    virtual std::string toString() const = 0;

    virtual std::string baseToString() const;
    virtual BaseAction* generate(BaseAction &other); // supposed to be =0 but comile error


protected:
    std::string cmd;

    void complete();

    void error(std::string errorMsg);

    std::string getErrorMsg() const;

private:
    std::string errorMsg;
    ActionStatus status = PENDING;
};

class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(OpenTable &other);

private:
    const int tableId;
    std::vector<Customer *> customers; // this supposed to point to Table.customers
};

class Order : public BaseAction {
public:
    Order(int id);

//    Order(int id, std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(Order &other);
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);

//    MoveCustomer(int src, int dst, int customerId, std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(MoveCustomer &other);

private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);

//    Close(int id, std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(Close &other);

private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();

//    CloseAll(std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(CloseAll &other);

private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();

//    PrintMenu(std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(PrintMenu &other);

private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);

//    PrintTableStatus(int id, std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(PrintTableStatus &other);

private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
//    PrintActionsLog(std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(PrintActionsLog &other);

private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();

//    BackupRestaurant(std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(BackupRestaurant &other);

private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();

//    RestoreResturant(std::string cmd);

    void act(Restaurant &restaurant);

    std::string toString() const;
    BaseAction* generate(RestoreResturant &other);

};


#endif
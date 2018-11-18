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

    BaseAction(const BaseAction &other);

    std::string endOfToString(ActionStatus actionStatus, std::string errMsg) const;

    virtual ~BaseAction();

    ActionStatus getStatus() const;

    virtual void act(Restaurant &restaurant) = 0;

    virtual std::string toString() const = 0;

    virtual BaseAction *clone() = 0;

    std::string getStatusValue(ActionStatus actionStatus) const;

protected:

    void complete();

    void error(std::string errorMsg);

    std::string getErrorMsg() const;

private:
    std::string errorMsg;
    ActionStatus status;
};

class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);

    OpenTable(const OpenTable &other);

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();

    ~OpenTable();

private:
    const int tableId;
    std::vector<Customer *> customers; // this supposed to point to Table.customers
};

class Order : public BaseAction {
public:
    Order(int id);

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();

private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();

private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();

private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();

private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();

    void act(Restaurant &restaurant);

    std::string getTypeString(DishType t);

    std::string toString() const;

    BaseAction *clone();

private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();

private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();

private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();

private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();

    void act(Restaurant &restaurant);

    std::string toString() const;

    BaseAction *clone();
};

#endif
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"

class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    Restaurant(const Restaurant &other); // copy con
    Restaurant& operator=(const Restaurant &other); // copy ass op
    Restaurant(Restaurant &&other); // move con
    Restaurant& operator=(Restaurant &&other); // move ass op
    virtual ~Restaurant();

    void cleanOther(Restaurant &other);
    void StealFromOtherToMe(const Restaurant &other);
    void copyFromOtherIntoMe(const Restaurant &other);
    void cleanMySelf();
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const;
    std::vector<Dish>& getMenu();
    std::vector<std::string> split(const std::string& s, char delimiter);
    std::vector<std::string> getLines(const std::string &configFilePath);
    int generateCustomerId();
    DishType getType(std::string &type_s);
    void closeRestuarant();
    void initiateCustomersByType(const std::vector<std::string> &words, std::vector<Customer *> &customers);

private:
    int nextCustomerId;
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;

    void handleCustomerIdsGeneration(int size);

    void cleanTempCustomers(std::vector<Customer *> &customers);
};

#endif
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
    virtual ~Restaurant();
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    std::vector<std::string> split(const std::string& s, char delimiter);
    std::vector<std::string> getLines(const std::string &configFilePath);
    int generateDishId();
    int generateCustomerId();
    DishType getType(std::string &type_s);
    bool startsWith(std::string a_string, std::string prefix);
    void print(std::string s, int i);
    void print(std::string s);
    void closeRestuarant();
    bool verifiedOpen(const std::vector<std::string> words);
    bool verifiedCmdTableNum(std::vector<std::string> words);
    bool VerifiedMove(std::vector<std::string> words);
    bool verifiedClose(std::vector<std::string> words);
    bool verifiedStatus(std::vector<std::string> words);
    bool isNumber(std::string s);
    int getTableId(std::vector<std::string> words);

private:
    int nextDishId = 0;
    int nextCustomerId = 0;
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    void initiateCustomersByType(const std::vector<std::string> &words, std::vector<Customer *> &customers);
};

#endif
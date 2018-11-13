#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    Table(const Table &other); //copy con
    Table& operator=(const Table &other); //copy ass op
    Table(Table &&other); // move con
    Table& operator=(Table &&other); // move ass op
    void cleanMySelf();
    void cleanOther(Table &other);
    void copyFromOtherIntoMe(const Table &other);
    void StealFromOtherToMe(const Table &other);
    virtual ~Table();


    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    void addOrders(std::vector<OrderPair> orders);
    void removeCustomerOrders(int id);
    std::vector<OrderPair> getCustomerOrders(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    Dish getDish(const std::vector<Dish> &menu, int d_id);
    void clearCustomers();

private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList;
    //  (  customer_id , Dish  )
    // A list of pairs for each order in a table
};


#endif
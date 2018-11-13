#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "SortableDish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    Customer(Customer &other);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;

    virtual Customer* generate(std::string name, int id) = 0;
    std::string getName() const;
    int getId() const;
    void sortById(std::vector<SortableDish> &vec);
    void sortByPrice(std::vector<SortableDish> &vec);
	void print(std::string s, int i);
	void print(std::string s);

private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
    virtual std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* generate(std::string name, int id);
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* generate(std::string name, int id);

private:
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
	SpicyCustomer(const SpicyCustomer &other);
    virtual std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* generate(std::string name, int id);
    bool hasOrdered();
    void setOrdered(bool newState);
private:
    bool ordered;
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
	AlchoholicCustomer(const AlchoholicCustomer &other);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* generate(std::string name, int id);
    int getDrinksHad();
    void increaseDrinksHad();
private:
    int drinksHad;

};


#endif
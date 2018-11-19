#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "SortableDish.h"

class Customer{
public:
    virtual ~Customer();
    Customer(std::string c_name, int c_id);

    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
	Customer& operator=(const Customer &other);
	virtual std::string getType()=0;
	virtual Customer* clone() = 0;
    std::string getName() const;
    int getId() const;
    void sortById(std::vector<SortableDish> &vec);
    void sortByPrice(std::vector<SortableDish> &vec);

private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
    virtual std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer *clone();
	std::string getType();
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer *clone();
	std::string getType();
    bool hasOrdered();
    void setOrdered(bool newState);

private:
    bool ordered;
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
    virtual std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer *clone();
    bool hasOrdered();
    void setOrdered(bool newState);
	std::string getType();
private:
    bool ordered;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer *clone();
	std::string getType();
    int getDrinksHad();
    void increaseDrinksHad();
private:
    int drinksHad;

};


#endif
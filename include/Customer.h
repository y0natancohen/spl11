#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "SortableDish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual ~Customer(); // warnings without this

    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
	virtual std::string getType() const = 0;
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
	std::string getType() const;
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer *clone();
	std::string getType() const;
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
	std::string getType() const;
private:
    bool ordered;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer *clone();
	std::string getType() const;
    void increaseDrinksHad();
private:
    int numberOfDrinksHad;

};


#endif
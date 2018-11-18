//
// Created by yonatan on 11/8/18.
//
#include <iostream>
#include <algorithm>
#include "../include/Customer.h"
#include "../include/Dish.h"
#include "../include/SortableDish.h"


Customer::Customer(std::string c_name, int c_id):
                name(c_name), id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

// VEG

VegetarianCustomer::VegetarianCustomer(std::string name, int id):
                    Customer(name, id) {}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    // vegetarian dish with the smallest id in the menu
    // the most expensive beverage (Non-alcoholic).
    std::vector<int> result;
    if (menu.size() <=  0){
        return result;
    }
    // veg
    std::vector<SortableDish> vegis;
    for (auto dish: menu){
        if (dish.getType() == VEG){
            vegis.push_back(SortableDish(dish));
        }
    }
    if (vegis.size() > 0){

        sortById(vegis);
        result.push_back(vegis[0].getId());
    }
    // drinks
    std::vector<SortableDish> drinks;
    for (auto dish: menu){
        if (dish.getType() == BVG){
            drinks.push_back(SortableDish(dish));
        }
    }
    if (drinks.size() > 0){
        sortByPrice(drinks);
        int highest_price = drinks[drinks.size() - 1].getPrice();
        std::vector<SortableDish> same_prices;
        for (auto drink: drinks){
            if (drink.getPrice() == highest_price){
                same_prices.push_back(drink);
            }
        }
        sortById(same_prices);
        result.push_back(same_prices[0].getId());
    }
    return result;
}


std::string VegetarianCustomer::toString() const {
    return getName();
}


Customer *VegetarianCustomer::clone() {
    return new VegetarianCustomer(*this);
}

std::string VegetarianCustomer::getType() {
    return "veg";
}

// CHEAP


CheapCustomer::CheapCustomer(std::string name, int id) :
                Customer(name, id) {}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    // cheapest dish in the menu
//    int dish = getCorrectId(menu, BVG, 0, false, true);
    std::cout << "cheap ordering"<< std::endl;
    std::vector<int> result;
    if (menu.size() <=  0){
        return result;
    }
    std::vector<SortableDish> dishes;
    for (auto dish: menu){
        dishes.push_back(SortableDish(dish));
    }
    if (dishes.size() > 0){
        sortByPrice(dishes);
        int lowest_price = dishes[0].getPrice();
        std::vector<SortableDish> same_prices;
        for (auto dish: dishes){
            if (dish.getPrice() == lowest_price){
                same_prices.push_back(dish);
            }
        }
        sortById(same_prices);
        result.push_back(same_prices[0].getId());
    }
    return result;
}

std::string CheapCustomer::toString() const {
    return getName();
}

Customer *CheapCustomer::clone() {
    return new CheapCustomer(*this);
}

std::string CheapCustomer::getType() {
    return "chp";
}


// SPICY


SpicyCustomer::SpicyCustomer(std::string name, int id) :
                Customer(name, id) {
    ordered = false;
}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::cout << "spicy ordering"<< std::endl;
    std::vector<int> result;
    if (menu.size() <=  0){
        return result;
    }

    if (hasOrdered()) {  // cheapest non-alcoholic beverage
        std::vector<SortableDish> dishes;
        for (auto dish: menu){
            if (dish.getType() == BVG){
                dishes.push_back(SortableDish(dish));
            }
        }
        if (dishes.size() > 0){
            sortByPrice(dishes);
            int lowest_price = dishes[0].getPrice();
            std::vector<SortableDish> same_prices;
            for (auto dish: dishes){
                if (dish.getPrice() == lowest_price){
                    same_prices.push_back(dish);
                }
            }
            sortById(same_prices);
            result.push_back(same_prices[0].getId());
        }

    }else{  // most expensive spicy dish
        std::vector<SortableDish> dishes;
        for (auto dish: menu){
            if (dish.getType() == SPC){
                dishes.push_back(SortableDish(dish));
            }
        }
        if (dishes.size() > 0){
            sortByPrice(dishes);
            int highest_price = dishes[dishes.size() -1].getPrice();
            std::vector<SortableDish> same_prices;
            for (auto dish: dishes){
                if (dish.getPrice() == highest_price){
                    same_prices.push_back(dish);
                }
            }
            sortById(same_prices);
            result.push_back(same_prices[0].getId());
        }
        setOrdered(true); // important
    }

    return result;
}

std::string SpicyCustomer::toString() const {
    return getName();
}

bool SpicyCustomer::hasOrdered() {
    return ordered;
}

void SpicyCustomer::setOrdered(bool newState) {
    ordered = newState;
}


Customer *SpicyCustomer::clone() {
    return new SpicyCustomer(*this);
}

std::string SpicyCustomer::getType() {
    return "spc";
}


// ALCOHOL

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) :
                Customer(name, id) {
    drinksHad = 0;
}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::cout << "alcoholic ordering"<< std::endl;

    // nth cheapest alcoholic
//    int dish = getCorrectId(menu, ALC, getDrinksHad(), false, false);
    std::vector<int> result;
    if (menu.size() <=  0){
        return result;
    }

    std::vector<SortableDish> alcohols;
    for (auto dish: menu){
        if (dish.getType() == ALC){
            alcohols.push_back(SortableDish(dish));
        }
    }
    if (alcohols.size() > 0){

        sortByPrice(alcohols);
        int nth_lowest_price;
        if (drinksHad >= alcohols.size()){
            nth_lowest_price = alcohols[alcohols.size() -1].getPrice(); //most expensive
        }else{
            nth_lowest_price = alcohols[drinksHad].getPrice(); // nth lowest
        }
        std::vector<SortableDish> same_prices;
        for (auto dish: alcohols){
            if (dish.getPrice() == nth_lowest_price){
                same_prices.push_back(dish);
            }
        }
        sortById(same_prices);
        result.push_back(same_prices[0].getId());

    }
    if (result.size() > 0){
        increaseDrinksHad();
    }
    return result;
}

std::string AlchoholicCustomer::toString() const {
    return getName();
}

int AlchoholicCustomer::getDrinksHad() {
    return drinksHad;
}

void AlchoholicCustomer::increaseDrinksHad() {
    ++drinksHad;
}

Customer *AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}

std::string AlchoholicCustomer::getType() {
    return "alc";
}

void Customer::sortById(std::vector<SortableDish> &vec) {
    std::sort(vec.begin(), vec.end(),
              [](const SortableDish& lhs, const SortableDish& rhs)
              {return lhs.getId()< rhs.getId();});
}


void Customer::sortByPrice(std::vector<SortableDish> &vec) {
    std::sort(vec.begin(), vec.end(),
              [](const SortableDish& lhs, const SortableDish& rhs)
              {return lhs.getPrice()< rhs.getPrice();});
}


Customer::~Customer() {

}




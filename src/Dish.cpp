//
// Created by yonatan on 11/8/18.
//

#include "../include/Dish.h"

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):
                id(d_id), name(d_name), price(d_price), type(d_type){}

//Dish::Dish(Dish &other):
//                id(other.getId()),
//                name(other.getName()),
//                price(other.getPrice()),
//                type(other.getType()) {}

//Dish::Dish& operator=(Dish&& other)
//{
//    if (&other != this) {
//
//    }
//    return *this;
//}

int Dish::getId() const {
    return id;
}

std::string Dish::getName() const {
    return name;
}

int Dish::getPrice() const {
    return price;
}

DishType Dish::getType() const {
    return type;
}

Dish &Dish::operator=(Dish &&) {
//    std::swap(cstring, other.cstring);
    return *this;
}

Dish::Dish(const Dish &other): id(other.getId()),
                            name(other.getName()),
                            price(other.getPrice()),
                            type(other.getType())
{}



//Dish &Dish::operator=(Dish &&) {
//    return Dish();
//}




//
// Created by yonatan on 11/10/18.
//

#include "../include/Dish.h"
#include "../include/SortableDish.h"



SortableDish::SortableDish(Dish &dish):
        id(dish.getId()),
        name(dish.getName()),
        price(dish.getPrice()),
        type(dish.getType())
        {}

int SortableDish::getId() const {
    return id;
}

std::string SortableDish::getName() const {
    return name;
}

int SortableDish::getPrice() const {
    return price;
}

DishType SortableDish::getType() const {
    return type;
}


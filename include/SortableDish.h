//
// Created by yonatan on 11/10/18.
//

#ifndef UNTITLED_SORTABLEDISH_H
#define UNTITLED_SORTABLEDISH_H
#include "Dish.h"

class SortableDish{
public:
    SortableDish(Dish &dish);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    DishType getType() const;

private:
    int id;
    std::string name;
    int price;
    DishType type;
};

#endif //UNTITLED_SORTABLEDISH_H

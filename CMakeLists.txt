cmake_minimum_required(VERSION 3.10)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled src/Main.cpp include/Restaurant.h include/Table.h include/Customer.h include/Action.h include/Dish.h src/Action.cpp src/Customer.cpp src/Dish.cpp src/Restaurant.cpp src/Table.cpp)
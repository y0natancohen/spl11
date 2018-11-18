# All Targets
all: rest


# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
rest: bin/Action.o bin/Customer.o bin/Dish.o bin/SortableDish.o bin/Restaurant.o bin/Main.o bin/Table.o
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/rest bin/Action.o bin/Customer.o bin/Dish.o bin/SortableDish.o bin/Restaurant.o bin/Main.o bin/Table.o
	@echo 'Finished building target: rest'
	@echo ' '


bin/Action.o: src/Action.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/Dish.o: src/Dish.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Dish.o src/Dish.cpp

bin/SortableDish.o: src/SortableDish.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/SortableDish.o src/SortableDish.cpp

bin/Restaurant.o: src/Restaurant.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

bin/Main.o: src/Main.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

bin/Table.o: src/Table.cpp
	g++ -g -Wall -std=c++11 -c -Iinclude -o bin/Table.o src/Table.cpp



#-g -Wall -std=c++11

#Clean the build directory
clean: 
	rm -f bin/*

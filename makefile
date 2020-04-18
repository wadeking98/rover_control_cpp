
all: controller rover

controller: controller.cpp
	g++ -pthread -o controller controller.cpp

rover: rover.cpp
	g++ -pthread -o rover rover.cpp



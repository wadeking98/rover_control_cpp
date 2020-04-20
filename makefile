
all: rover controller clean

controller: controller.cpp rcp.o
	g++ controller.cpp rcp.o -pthread -o controller

rover: rover.cpp rcp.o
	g++ rover.cpp rcp.o -pthread -o rover 

rcp.o: rcp.cpp rcp.h
	g++ -c rcp.cpp rcp.h

clean:
	rm *.o *.gch



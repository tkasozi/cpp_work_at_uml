G++FLAGS = -std=c++0x -save-temps -g
CC = g++

proddonuts consdonuts: donuts.hpp globals.hpp producer.o consumer.o utilities.o
	$(CC) -o proddonuts producer.o utilities.o
	$(CC) -o consdonuts consumer.o utilities.o
	
consumer.o: donuts.hpp globals.hpp consumer.cpp
	$(CC) $(G++FLAGS) -c consumer.cpp
	
producer.o: donuts.hpp globals.hpp producer.cpp
	$(CC) $(G++FLAGS) -c producer.cpp
	
utilities.o:  utilities.cpp donuts.hpp globals.hpp
	$(CC) $(G++FLAGS) -c utilities.cpp
clean:
	rm c1 c2 c3 c4 c5 c6 c7 *.txt *.o *.s *.ii *~ proddonuts consdonuts
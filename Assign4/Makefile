G++FLAGS = -std=c++0x 
CC = g++
#to include/define GLOBAL
pc_pthread: globals.hpp Assign4_pc_pthread.o
	$(CC) -o pc_pthread Assign4_pc_pthread.o -lpthread 
Assign4_pc_pthread.o: globals.hpp Assign4_pc_pthread.cpp
	$(CC) $(G++FLAGS) -c Assign4_pc_pthread.cpp	
clean:
	rm *.o pc_pthread *~ *0 *1 *2 *3 *4 *5 *6 *7 *8 *9	
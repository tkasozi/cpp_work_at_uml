G++FLAGS = -std=c++0x 
CC = g++

Assign6:    Assign6.o
	    $(CC) $(G++FLAGS) -D_FILE_OFFSET_BITS=64 -g -o Assign6 Assign6.o 

Assign6.o:   Assign6.cpp
	    $(CC) $(G++FLAGS) -c Assign6.cpp
clean:
	-rm *.o
	-rm *~
	-rm Assign6
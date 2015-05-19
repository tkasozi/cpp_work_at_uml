#Makefile
#Talik A Kasozi
#talik.aziizi@gmail.com

G++FLAGS = -std=c++11 
CC = g++

run:	assign5
	
assign5:    Assign5.o FirstFit.o BestFit.o Utility.o BuddySys.o
	    $(CC) $(G++FLAGS)  -L. -lData_Struct_DLL  -o assign5 Assign5.o  Utility.o BestFit.o FirstFit.o BuddySys.o 
	
Assign5.o:  Assign5.hpp Assign5.cpp
	    $(CC) $(G++FLAGS) -c Assign5.cpp
	
FirstFit.o: Assign5.hpp FirstFit.cpp 
	    $(CC) $(G++FLAGS) -c FirstFit.cpp
	
BestFit.o:  Assign5.hpp BestFit.cpp 
	    $(CC) $(G++FLAGS) -c BestFit.cpp

Utility.o:  Assign5.hpp Utility.cpp
	    $(CC) $(G++FLAGS) -c Utility.cpp

BuddySys.o:  Assign5.hpp BuddySys.cpp
	    $(CC) $(G++FLAGS) -c BuddySys.cpp
	
clean:
	-rm *.o
	-rm *~
	-rm assign5
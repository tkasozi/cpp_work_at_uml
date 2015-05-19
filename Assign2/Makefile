CCFLAGS = -Wall
LFLAGS = -Wall -c
CC = 	g++
all: 	sort grep

sort:	pipe.o sort.o 
	$(CC) $(CCFLAGS) pipe.o sort.o -o sort

grep: 	pipe.o grep.o
	$(CC) $(CCFLAGS) pipe.o grep.o -o grep

sort.o:	globals.hpp sort.cpp
	$(CC) $(LFLAGS) sort.cpp

grep.o:	globals.hpp grep.cpp
	$(CC) $(LFLAGS) grep.cpp

pipe.o:	globals.hpp pipe.cpp
	$(CC) $(LFLAGS) pipe.cpp

clean:
	-rm *.ii
	-rm *.s
	-rm *.o
	-rm *.h.gch
	-rm *#
	-rm *~
	-rm grep
	-rm sort
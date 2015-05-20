//Talik Kasozi
//Modified: Sep 9,2014
//91.308

#ifndef GLOBALS_H
#define GLOBALS_H

#include<sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/signal.h>
#include <exception>
#include<time.h>

class Exception : public std::exception {
public:

    //Exception ;constructor

    Exception(const char* str) : err(str) {

    };

    virtual const char* what()const throw () {

        return err;
    }
private:
    const char *err;

};

#define		SEMKEY			(key_t)351295105
#define		MEMKEY			(key_t)796438652
#define		NUMFLAVORS	 	4
#define		NUMSLOTS       		47 //About 50% deadlock graphing consumers vs Deadlock
#define		NUMSEMIDS	 	3
#define		PROD		 	0
#define		CONSUMER	 	1
#define		OUTPTR		 	2

extern int p(int, int);
extern int v(int, int);
extern int semsetall(int, int, int);

extern void sig_handler(int);

#endif

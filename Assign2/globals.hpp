//Talik Kasozi
//
//Modified: Sep 9,2014
//91.308

#ifndef GLOBALS_H
#define GLOBALS_H

#include <exception>
#include <sys/types.h>
#include <unistd.h> 
#include <csignal>
#include <sys/resource.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define sizeofArray( array ) ( sizeof( array )/sizeof( array[0] ) )

static int i = 0;

typedef enum _Status {
    OK = 1, ERROR = 0
} status;

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

typedef union {
    int exit_status;

    struct {
        unsigned sig_num : 7;
        unsigned core_dmp : 1;
    } parts;
    unsigned exit_num : 8;
} LE_Wait_Status;

//Pipe
void sig_handler(int signal);

class Pipe {
    int _pipe[2];
public:
    Pipe();
    ~Pipe();
    int* get_pipe();
    void make_pipe();
    status _Read(char[], int*);
    status _write(const char*, int);
    void close_pipeR(const char* = "");
    void close_pipeW(const char* = "");
};

#endif

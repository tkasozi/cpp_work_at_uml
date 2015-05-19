#include "globals.hpp"
#include <string.h>
#include <stdio.h>
#include <iosfwd>
#include <string>

Pipe::Pipe() {
    //default
}

int* Pipe::get_pipe() {
    return _pipe;
}

//pipe()

void Pipe::make_pipe() {
    (pipe(_pipe) < 0) ?
            throw Exception("Exception was thrown while creating pipe.")
            :
            NULL
            ;
}

void Pipe::close_pipeR(const char* str) {
    std::string s = "Exception was thrown while closing the Read end of ";
    s.append(str);

    (close(_pipe[0]) < 0) ?
            throw Exception(s.c_str())
            :
            NULL
            ;
}

void Pipe::close_pipeW(const char* str) {
    std::string s = "Exception was thrown while closing the Write end of ";
    s.append(str);

    (close(_pipe[1]) < 0) ?
            throw Exception(s.c_str())
            :
            NULL
            ;
}

status Pipe::_write(const char* str, int nread) {

    if (write(Pipe::_pipe[1], str, nread) < 0)
        throw Exception("Exception while trying to write.");
    return OK;
}

status Pipe::_Read(char buf[], int* nread) {
    if ((*nread = read(_pipe[0], buf, BUF_SIZE)) > 0)

        return OK;

    else
        return ERROR;

}

Pipe::~Pipe() {

}
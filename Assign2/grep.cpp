/* 
 * File:   main.cpp
 * Author: Talik Kasozi
 *
 * Created on September 25, 2014, 5:11 PM
 */

#include <cstdlib>
#include <iostream>
#include "globals.hpp"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    char buf[BUF_SIZE] = { 0 }, buf2[BUF_SIZE] = { 0 }; 
    Pipe _parent, _child2;
    int fd, nread, n;

    try {
        _child2.make_pipe();
        _parent.make_pipe();

        switch (fork()) { //Child
            case -1:
                throw Exception("Error occured while creating a child. ");
            case 0:

                if (close(0) < 0 || close(1) < 0)
                    throw Exception("Exception on pipe");

                if (dup2(_parent.get_pipe()[0], STDIN_FILENO) < 0 || (dup2(_child2.get_pipe()[1], STDOUT_FILENO) < 0)) {
                    throw Exception("Exception while cleaning up the channels for communication btn Parent and Child");
                }

                _child2.close_pipeR("_pipeIn");
                _parent.close_pipeW("_pipeOut");
                _parent.close_pipeR("_pipeOut");
                _child2.close_pipeW("_pipeIn");

                execlp("grep", "grep", "123", NULL);
                throw Exception("Exception on execlp");

            default:
                //Parent
                _parent.close_pipeR("_pipeOut");
                _child2.close_pipeW("_PipeIn");

                if ((fd = open("cs308a2_grep_data_1", O_RDONLY, 0)) == -1)
                    throw Exception("Exception on FILE");
                else {

                    while ((nread = read(fd, buf, 80)) != 0) { //reading file.

                        if (nread < 0) throw Exception("Read Exception.");

                        if (_parent._write(buf, nread) == ERROR) //
                            throw Exception("Exception Writing while reading..");
                    }

                    if (close(fd) < 0 || close(_parent.get_pipe()[1]) < 0) throw Exception("close"); //won't know that it's finished

                    int count = 0;

                    while (_child2._Read(buf2, &n) != ERROR) { 

                        if (n < 0) throw Exception("Read Exception.");

                        for(int i =0; i < n; i++){
                            if(buf2[i]=='\n') ++count; //not accurate if the number appears twice in one string. 
                            
                            cout<< buf2[i]; 
                        }
                        //if (write(1, buf, n) == -1)

                    }
                    
                    cout << "\nThe number of lines w/ the matched number is " <<count <<endl;

                    if (close(_child2.get_pipe()[0]) < 0) throw Exception("close");

                }

        }

    } catch (Exception &e) {
        cout << e.what() << endl;
        exit(1); //just exit 
    }

    return 0;
} 
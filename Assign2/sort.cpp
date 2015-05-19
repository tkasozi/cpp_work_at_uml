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
#include <algorithm>    // std::count
#include <vector>  
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    char buf2[BUF_SIZE] = {0}, buf[BUF_SIZE] = {0};
    Pipe _parent, _child;
    int fd2, nread, population = 0, areacode = 0, zip, part1, part2;
    string first, last, line;

    bool flag = true;


    ofstream myfile;
    myfile.open("output.txt", std::ofstream::out);

    ifstream myfile2("output.txt", std::ifstream::in);

    try {
        _child.make_pipe();
        _parent.make_pipe();

        switch (fork()) { //Child 
            case -1:
                throw Exception("Error occured while creating a child. ");
            case 0:

                if (close(0) < 0 || close(1) < 0)
                    throw Exception("Exception on pipe");

                if (dup2(_parent.get_pipe()[0], STDIN_FILENO) < 0 || (dup2(_child.get_pipe()[1], STDOUT_FILENO) < 0)) {
                    throw Exception("Exception while cleaning up the channels for communication btn Parent and Child");
                }

                _parent.close_pipeW("_parent");
                _parent.close_pipeR("_parent");
                _child.close_pipeW("_child");
                _child.close_pipeR("_child");


                execlp("sort", "sort", "-k 3.3n", "-k 2.2", NULL); //

                throw Exception("Exception on execlp");

            default:
                _parent.close_pipeR("_pipeOut");
                _child.close_pipeW("_PipeIn");

                if ((fd2 = open("cs308a2_sort_data", O_RDONLY, 0)) == -1)
                    throw Exception("Exception on FILE");
                else {

                    while ((nread = read(fd2, buf2, sizeof (buf2))) != 0) { //reading file.

                        if (nread < 0) throw Exception("Read Exception.");

                        if (_parent._write(buf2, nread) == ERROR) //_pipeOut[1] 
                            throw Exception("Exception Writing while reading..");
                    }
                    if (close(fd2) < 0 || close(_parent.get_pipe()[1]) < 0) throw Exception("close");

                    while (_child._Read(buf, &nread) != ERROR) {

                        for (int i = 0; i < nread; i++) {
                            myfile << buf[i];
                        }
                    }
                    myfile.close();

                    while (myfile2 >> first >> last >> zip >> part1 >> part2) {

                        int temp = zip;

                        if (flag) {
                            areacode = zip; //on time
                            flag = false;
                        }
                        if (areacode == temp) {
                            ++population;
                        } else {
                            cout << areacode << " Was found " << population << " times" << endl;
                            population = 0; //b'se it's already sorted ˙.˙ if != areacode then it's new areacode
                            areacode = temp;
                        }
                    }
                    myfile2.close();

                    if (close(_child.get_pipe()[0]) < 0) throw Exception("close");

                }
                break;
        }
    } catch (Exception &e) {
        cout << e.what() << endl;
    }
    return 0;
}
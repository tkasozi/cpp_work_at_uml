/* 
 * File:   Assign5.hpp
 * Author: Talik Kasozi
 *
 * request struct from ~bill/cs308 folder
 * 
 * Created on November 6, 2014, 12:10 PM
 */

#ifndef ASSIGN5_HPP
#define	ASSIGN5_HPP
#include "includes/List.hpp"

#include <iostream>
#include <unistd.h> 
#include <fcntl.h>
#include <sstream>
#include <cstdlib>

#define NUMBER_ENTRIES  (1001)
#define FAILED  (0)
#define DONE (1)

 struct request {
    bool is_req;
    bool is_allocated;
    int status;
    int size;
    int base_adr;
    int next_boundary_adr;
    int memory_left;
    int largest_chunk;
    int elements_on_free_list;
    int block_size;
    int block_adr;
    int adjacent_adr;
};

struct free_list {
    struct free_list *next;
    struct free_list *previous;
    int block_size;
    int block_adr;
    int adjacent_adr;
    int leftover;
};

struct info {
    int block_size;
    int block_adr;
    int adjacent_adr;
};

void best_Fit(int mem, std::string str);
void first_Fit(int mem, std::string str);
void printd(request [], std::string , int );
void buddySys(int memorySize, std::string);
int update_list(int i, request[], free_list, int*) ;

#endif	/* ASSIGN5_HPP */
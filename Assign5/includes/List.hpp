/* 
 * File:   List.hpp
 * Author: apple
 *
 * Created on October 21, 2014, 4:55 PM
 * last update: Oct 26, 2014 12:22AM
 */

#ifndef LIST_HPP
#define	LIST_HPP

#include "globals.hpp"

#define DATA(ptr) ((ptr)->data)
#define NEXT(ptr) ((ptr)->next)
#define PREVIOUS(ptr) ((ptr)->previous)

enum status {
    ERROR = 0, OK = 1, FIFO = 2, LIFO = 3
};

typedef struct node { //protect this in the next version
    void* data;
    node* next;
    node* previous;
} *list_ptr;

class List {
protected:
    status delete_node();
public:
    void ctest2(int *i);
    int length();
    status delete_list();
    void set_listptr(list_ptr ptr);
    void traverse(void (*func)(generic_ptr)); //sign. changed 11/6 12:18A
    list_ptr _list();
    void set_Orientation(status);
    generic_ptr peek(); //changed 11/5 11:30PM FROM: generic_ptr get_DATA(list_ptr);
    bool empty();
    status get_List_Status();
    status insert(generic_ptr);
    List();

    ~List();
private:
    //allocate node
    list_ptr allocate_node(generic_ptr data_ptr);
    status init_List();
    status List_status;
    int count;
    list_ptr last, list; //can help impl. a circular list

};

#endif	/* LIST_HPP */


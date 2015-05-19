#include "Assign5.hpp"

using namespace std;

int allocate_memory(struct request *, free_list list_head, int*);
int x = 0;

int allocate_memory(struct request * request, free_list list_head, int* total_free) {

    struct free_list* free_l = NULL;


    //cout << ++x << ": size " << *total_free << endl;


    for (free_l = list_head.next; free_l; free_l = free_l -> next) {
        if (request -> size <= free_l -> block_size) {

            //Valid
            request -> is_allocated = true;
            request->status = DONE;
            request -> base_adr = free_l -> block_adr;
            request -> next_boundary_adr = request -> base_adr + request -> size;

            *total_free = *total_free - request -> size;
            request -> memory_left = *total_free;

            if ((free_l -> block_size = free_l -> block_size - request -> size) == 0) {
                free_l -> previous -> next = free_l -> next;
                free_l -> next -> previous = free_l -> previous;

                // found valid block 
                free(free_l);
                return 0;
            }

            free_l -> block_adr = free_l -> block_adr + request -> size;
            return 0;
        }
    }
    request->status = FAILED;
    request -> memory_left = *total_free;

    return 0;
}

void first_Fit(int mem, string str) {
    int serial_num, size_or_free_req, total_free_space, total_free;
    string req;
    FILE *File;
    char line[80];
    request req_array[NUMBER_ENTRIES];

    struct free_list list_head_, *top;
    free_list* i;


    //List *top = new List();
    //List *list_head = new List();

    //top->set_Orientation(FIFO);

    File = fopen(str.c_str(), "r");

    //initialization of elements of the  
    for (int i = 0; i <= NUMBER_ENTRIES; i++) {
        req_array[i].is_req = false;
        req_array[i].is_allocated = false;
    }

    //        req_info[i].block_size = -1;

    total_free_space = total_free = (mem * 1024);

    cout << "TOTAL CHUNK: " << total_free_space << "\n" << endl;

    top = (free_list*) malloc(sizeof (struct free_list));

    top -> next = NULL;
    top -> previous = &list_head_;
    top -> block_size = total_free_space;
    top -> block_adr = 0;
    top -> adjacent_adr = total_free_space;

    /* set the list to the first free node */
    list_head_.next = top;
    list_head_.previous = NULL;
    list_head_.block_size = -1;

    while (fgets(line, 80, File) != NULL) {
        stringstream ss;
        ss << line;

        ss >> serial_num >> req >> size_or_free_req;

        if (req.compare("alloc") == 0) {
            //try Allocate Memory
            req_array[serial_num].size = size_or_free_req;
            req_array[serial_num].is_req = true;

            allocate_memory(&req_array[serial_num], list_head_, &total_free);

            req_array[serial_num].elements_on_free_list = 0;
            req_array[serial_num].largest_chunk = 0;

            for (i = list_head_.next; i != NULL; i = i->next) {

                ++req_array[serial_num].elements_on_free_list;

                if (i -> block_size > req_array[serial_num].largest_chunk) {
                    req_array[serial_num].largest_chunk = i -> block_size;
                }

            }
        } else {
            //free Memory

            req_array[serial_num].size = req_array[size_or_free_req].size;
            req_array[serial_num].is_allocated = req_array[size_or_free_req].is_allocated;

            update_list(size_or_free_req, req_array, list_head_, &total_free);

            // Request complete, reset values 
            req_array[serial_num].memory_left = total_free;
            req_array[serial_num].elements_on_free_list = 0;
            req_array[serial_num].largest_chunk = 0;


            for (i = list_head_.next; i != NULL; i = i->next) {

                ++req_array[serial_num].elements_on_free_list;

                // get the largest chunk for the request 
                if (i -> block_size > req_array[serial_num].largest_chunk) {
                    req_array[serial_num].largest_chunk = i -> block_size;
                }
            }

        }

    }

    printd(req_array, "First Fit", mem);
    fclose(File);
}

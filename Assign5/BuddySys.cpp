#include "Assign5.hpp"

//request req_array[NUMBER_ENTRIES];

using namespace std;

struct info_ {
    int diff;
    free_list *addr;
};

int allocateMemoryBuddySys(struct request * request, free_list list_head, int* total_free) {

    info_ * inf [100];

    for (int i = 0; i < 100; i++) {
        inf[i] = (info_*) malloc(sizeof ( struct info_));
        inf[i]->diff = 0;
    }

    int y = 0;
    struct free_list* free_l = NULL;
    struct free_list* valid_l = NULL;

    int Diff = 0;
    int mem_left = 0;

    /* Do this the first time we do the loop */
    bool check = true;

    /* Find the smallest block that fits on the free list */
    for (free_l = list_head.next; free_l; free_l = free_l -> next) {
        if (request -> size <= free_l -> block_size) {

            Diff = free_l -> block_size - request -> size;

            if (check) mem_left = Diff;

            check = false;

            /* found smaller chunk */
            if (mem_left >= Diff) {
                mem_left = Diff;
                valid_l = free_l;
            }

        }
    }

    /* Did we find stuff we can use */
    if (valid_l != NULL) {
        /* Set request */
        request -> is_allocated = true;
        request->status = DONE;
        request -> base_adr = valid_l -> block_adr;
        request -> next_boundary_adr = request -> base_adr + request -> size;

        /* update remaining space */
        *total_free -= request -> size;
        request -> memory_left = *total_free;

        /* If there is a perfect match, need to handle it.*/
        if ((valid_l -> block_size = valid_l -> block_size - request -> size) == 0) {
            valid_l -> previous -> next = valid_l -> next;
            valid_l -> next -> previous = valid_l -> previous;

            free(valid_l);
            return 0;
        }

        valid_l -> block_adr = valid_l -> block_adr + request -> size;
        return 0;

    }

    /* Did not find a valid space */
    request -> memory_left = *total_free;
    request->is_allocated = false;

    return 0;
}

void buddySys(int mem, string str) {

    int serial_num, size_or_free_req, total_free_space, total_free;
    string req;
    FILE *File;
    char line[80];
    request req_array[NUMBER_ENTRIES];

    struct free_list list_head_, *top;

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

            //  cout << "after head links " << list_head->length() << endl;

            allocateMemoryBuddySys(&req_array[serial_num], list_head_, &total_free);

            req_array[serial_num].elements_on_free_list = 0;
            req_array[serial_num].largest_chunk = 0;

            for (free_list* i = list_head_.next; i != NULL; i = i->next) {

                ++req_array[serial_num].elements_on_free_list;

                if (i -> block_size > req_array[serial_num].largest_chunk) {
                    req_array[serial_num].largest_chunk = i -> block_size;
                }

            }

        } else {//free Memory

            req_array[serial_num].size = req_array[size_or_free_req].size;
            req_array[serial_num].is_allocated = req_array[size_or_free_req].is_allocated;

            update_list(size_or_free_req, req_array, list_head_, &total_free);

            // Request complete, reset values 
            req_array[serial_num].memory_left = total_free;
            req_array[serial_num].elements_on_free_list = 0;
            req_array[serial_num].largest_chunk = 0;


            for (free_list* i = list_head_.next; i != NULL; i = i->next) {

                ++req_array[serial_num].elements_on_free_list;

                // get the largest chunk for the request 
                if (i -> block_size > req_array[serial_num].largest_chunk) {
                    req_array[serial_num].largest_chunk = i -> block_size;
                }
            }
        }

    }

    printd(req_array, "Best Fit", mem);
    fclose(File);

}


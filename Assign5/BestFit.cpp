#include "Assign5.hpp"

using namespace std;

int total_free_space;
int total_free;

request req_array[NUMBER_ENTRIES ];

int allocateMemoryBest(struct request* request, free_list list_head) {

    struct free_list* free_l = NULL;
    struct free_list* valid = NULL;

    int diff = 0;
    int memleft = 0;
    
    bool check = true;
    

    /* Find the smallest block that fits on the free list */
    for (free_l = list_head.next; free_l; free_l = free_l -> next) {
        if (request -> size <= free_l -> block_size) {
            diff = free_l -> block_size - request -> size;

            request->memory_left = diff;


            //only once
            if (check)
                memleft = request->memory_left;
            
            check = false;

            //            printf("size difference, %d\n", sizeDiff);

            /* found smaller chunk */
            if (memleft >= diff) {
                memleft = diff;
                valid = free_l;
            }

        }
    }

    /* Did we find stuff we  can use */
    if (valid != NULL) {
        /* Set request */
        request -> is_allocated = true;
        request->status = DONE;
        request -> base_adr = valid -> block_adr;
        request -> next_boundary_adr = request -> base_adr + request -> size;

        /* update remaining space */
        total_free = total_free - request -> size;
        request -> memory_left = total_free;


        /* If there is a perfect match, need to handle it.*/
        if ((valid->block_size = valid->block_size - request -> size) == 0) {
            valid -> previous -> next = valid -> next;
            valid -> next -> previous = valid -> previous;
            delete(valid);
            return 0;
        }

        valid -> block_adr = valid -> block_adr + request -> size;

        return 0;

    }

    /* Did not find a valid space */
    request -> memory_left = total_free;
    request->status = FAILED;
    return 0;

}

void best_Fit(int memorySize, std::string str) {

    int i = 0;
    int requestSeq = 0;
    int requestSize = 0;
    string requestType;
    char line[80];

    struct free_list* top, list_head;

    /* Write to this file */
    FILE *file = fopen(str.c_str(), "r");


    /* The list of free space */
    struct free_list* freeList;

    /* Total amount of free space */
    total_free_space = total_free = (memorySize * 1024);

    /* init request array */
    for (i = 0; i < NUMBER_ENTRIES; i++) {
        req_array[i].is_req = false;
        req_array[i].is_allocated = false;
    }

    /* First free node setup*/
    /* this struct is from the help file */
    top = (free_list*) malloc(sizeof (struct free_list));

    top -> next = NULL;
    top -> previous = &list_head;
    top -> block_size = total_free_space;
    top -> block_adr = 0;
    top -> adjacent_adr = total_free_space;

    /* set the list to the first free node */
    list_head.next = top;
    list_head.previous = NULL;
    list_head.block_size = -1;

    /* get the requests */
    while (fgets(line, 80, file) != NULL) {
        stringstream ss;

        ss << line;

        ss >> requestSeq >> requestType >> requestSize;

        /* Allocation? */
        if (requestType.compare("alloc") == 0) {

            /* Create request */
            req_array[requestSeq].is_req = true;
            req_array[requestSeq].size = requestSize;

            allocateMemoryBest(&req_array[requestSeq], list_head);

            /* Request complete, reset values */
            req_array[requestSeq].elements_on_free_list = 0;
            req_array[requestSeq].largest_chunk = 0;

            for (freeList = list_head.next; freeList; freeList = freeList -> next) {
                /* get number of free things */
                ++req_array[requestSeq].elements_on_free_list;

                /* get the largest chunk for the request */
                if (freeList -> block_size > req_array[requestSeq].largest_chunk) {
                    req_array[requestSeq].largest_chunk = freeList -> block_size;
                }
            }

        } else {
            /* Free operation */
            req_array[requestSeq].size = req_array[requestSize].size;
            req_array[requestSeq].is_allocated = req_array[requestSize].is_allocated;

            update_list(requestSize, req_array, list_head, &total_free);
            
            /* Request complete, reset values */
            req_array[requestSeq].memory_left = total_free;
            req_array[requestSeq].elements_on_free_list = 0;
            req_array[requestSeq].largest_chunk = 0;

            for (freeList = list_head.next; freeList; freeList = freeList -> next) {
                /* get number of free things */
                ++req_array[requestSeq].elements_on_free_list;

                if (freeList -> block_size > req_array[requestSeq].largest_chunk) {
                    req_array[requestSeq].largest_chunk = freeList -> block_size;
                }
            }

        }

    }

    printd(req_array, "Best System", memorySize);

    /* All set close it out */
    fclose(file);
}
